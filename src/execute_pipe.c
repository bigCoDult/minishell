/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:31:16 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/25 11:42:12 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int handle_heredocs_recursive(t_shell *shell, t_ast_node *node)
{
    if (!node)
        return (0);

    // 현재 노드가 명령어인 경우
    if (node->type == AST_COMMAND)
    {
        t_redirection *redir = node->cmd->redirs;
        while (redir)
        {
            if (redir->type == REDIR_HEREDOC)
            {
                if (handle_heredoc(shell, redir->filename) != 0)
                    return (1);
            }
            redir = redir->next;
        }
        return (0);
    }

    // 파이프 노드인 경우 양쪽 모두 처리
    if (node->type == AST_PIPE)
    {
        if (handle_heredocs_recursive(shell, node->left) != 0)
            return (1);
        return handle_heredocs_recursive(shell, node->right);
    }

    return (0);
}

void setup_pipe(t_shell *shell, int pipefd[2], int is_first, int is_last)
{
    if (!is_first)  // 첫 번째 명령어가 아니면
    {
        dup2(shell->pipe_info.prev_pipe, STDIN_FILENO);  // 이전 파이프의 읽기를 표준 입력으로
        close(shell->pipe_info.prev_pipe);
    }
    
    if (!is_last)  // 마지막 명령어가 아니면
    {
        dup2(pipefd[1], STDOUT_FILENO);  // 현재 파이프의 쓰기를 표준 출력으로
    }
    
    close(pipefd[0]);
    close(pipefd[1]);
}

int execute_piped_command(t_shell *shell, t_command *cmd)
{
    pid_t pid;
    
    pid = fork();
    if (pid == 0)
    {
        // 이전 파이프에서 입력 받기
        if (shell->pipe_info.prev_pipe != -1)
        {
            dup2(shell->pipe_info.prev_pipe, STDIN_FILENO);
            close(shell->pipe_info.prev_pipe);
        }
        
        // 다음 명령어로 출력 전달
        if (!shell->pipe_info.is_last)
        {
            dup2(shell->pipe_info.pipefd[1], STDOUT_FILENO);
        }
        
        // 파이프 fd 정리
        close(shell->pipe_info.pipefd[0]);
        close(shell->pipe_info.pipefd[1]);

        // 명령어 실행
        char *executable_path = find_command_path(shell, cmd->args[0]);
        if (!executable_path)
            // exit(127);
			free_exit(shell, 127);

        char **env_array = get_env_array(shell);
        if (!env_array)
            // exit(1);
			free_exit(shell, 1);

        execve(executable_path, cmd->args, env_array);
        // exit(127);
		free_exit(shell, 127);
    }
    
    // 부모 프로세스에서 파이프 fd 정리
    if (shell->pipe_info.prev_pipe != -1)
        close(shell->pipe_info.prev_pipe);
    close(shell->pipe_info.pipefd[1]);
    
    return pid;
}

// 히어독 처리 함수 추가
static int handle_all_heredocs(t_shell *shell, t_ast_node *node)
{
    int original_stdin;
    int debug_fd;
    int result;
    
    original_stdin = dup(STDIN_FILENO);
    if (original_stdin == -1)
        return (1);

    debug_fd = dup(STDOUT_FILENO);
    if (debug_fd == -1)
    {
        close(original_stdin);
        return (1);
    }

    // 히어독 처리 전에 원본 stdin 저장
    shell->heredoc.original_stdin = original_stdin;
    
    result = handle_heredocs_recursive(shell, node);

    // 디버그 출력 복원
    dup2(debug_fd, STDOUT_FILENO);
    close(debug_fd);

    return result;
}

int execute_pipe(t_shell *shell, t_ast_node *node)
{
    int pipefd[2];
    pid_t pid1, pid2;
    int status1, status2;

    if (!node || !node->left || !node->right)
        return (1);

    // 히어독 처리
    if (handle_all_heredocs(shell, node) != 0)
        return (1);

    // 파이프 생성
    if (pipe(pipefd) == -1)
        return (1);

    // 첫 번째 자식 프로세스 (왼쪽 명령어)
    pid1 = fork();
    if (pid1 == 0)
    {
        int heredoc_fd = -1;

        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (node->left->type == AST_COMMAND)
        {
            heredoc_fd = find_command_heredoc_fd(shell, node->left->cmd);
            if (heredoc_fd != -1)
            {
                lseek(heredoc_fd, 0, SEEK_SET);
                dup2(heredoc_fd, STDIN_FILENO);
                close(heredoc_fd);
            }

            if (is_builtin(node->left->cmd->args[0]))
            {
                int ret = execute_builtin(shell, node->left->cmd);
                free_exit(shell, ret);
            }
            else
            {
                char *path = find_command_path(shell, node->left->cmd->args[0]);
                if (!path)
                    free_exit(shell, 127);
                execve(path, node->left->cmd->args, get_env_array(shell));
                free_exit(shell, 127);
            }
        }
        free_exit(shell, 1);
    }

    // 두 번째 자식 프로세스 (오른쪽 명령어/파이프)
    pid2 = fork();
    if (pid2 == 0)
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        if (node->right->type == AST_PIPE)
        {
            int ret = execute_pipe(shell, node->right);
            exit(ret);  // free_exit 대신 일반 exit 사용
        }
        else if (node->right->type == AST_COMMAND)
        {
            int heredoc_fd = find_command_heredoc_fd(shell, node->right->cmd);
            if (heredoc_fd != -1)
            {
                lseek(heredoc_fd, 0, SEEK_SET);
                dup2(heredoc_fd, STDIN_FILENO);
                close(heredoc_fd);
            }

            if (is_builtin(node->right->cmd->args[0]))
            {
                int ret = execute_builtin(shell, node->right->cmd);
                free_exit(shell, ret);
            }
            else
            {
                char *path = find_command_path(shell, node->right->cmd->args[0]);
                if (!path)
                    free_exit(shell, 127);
                execve(path, node->right->cmd->args, get_env_array(shell));
                free_exit(shell, 127);
            }
        }
        free_exit(shell, 1);
    }

    // 부모 프로세스
    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);

    return WEXITSTATUS(status2);
}