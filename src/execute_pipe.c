/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:31:16 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/18 22:25:55 by sanbaek          ###   ########.fr       */
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
    
    // 표준 입력 저장
    original_stdin = dup(STDIN_FILENO);
    if (original_stdin == -1)
        return (1);

    // 디버그 출력을 임시로 비활성화
    int debug_fd = dup(STDOUT_FILENO);
    if (debug_fd == -1)
        return (1);

    // 재귀적으로 모든 히어독 처리
    int result = handle_heredocs_recursive(shell, node);

    // 표준 입력/출력 복원
    dup2(original_stdin, STDIN_FILENO);
    dup2(debug_fd, STDOUT_FILENO);
    close(original_stdin);
    close(debug_fd);

    return result;
}

int execute_pipe(t_shell *shell, t_ast_node *node)
{
    int pipefd[2];
    pid_t pid1, pid2;
    int status1, status2;

    debug_print(2047, 8, "\n=== EXECUTE PIPE ===\n");
    
    if (!node || !node->left || !node->right)
        return (1);

    // 모든 히어독을 먼저 처리
    if (handle_all_heredocs(shell, node) != 0)
        return (1);

    if (pipe(pipefd) == -1)
        return (1);

    // 첫 번째 명령어 실행 (왼쪽 노드)
    pid1 = fork();
    if (pid1 == 0)
    {
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
            // exit(1);
			free_exit(shell, 1);
        close(pipefd[1]);
        
        if (node->left->type == AST_COMMAND)
        {
            debug_print(2047, 8, "Executing left command: %s\n", node->left->cmd->args[0]);
            // exit(execute_simple_command(shell, node->left->cmd));
			free_exit(shell, execute_simple_command(shell, node->left->cmd));
        }
        // exit(1);
		free_exit(shell, 1);
    }

    // 두 번째 명령어 실행 (오른쪽 노드)
    pid2 = fork();
    if (pid2 == 0)
    {
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
            // exit(1);
			free_exit(shell, 1);
        close(pipefd[0]);
        
        if (node->right->type == AST_PIPE)
        {
            debug_print(2047, 8, "Executing right pipe\n");
            // exit(execute_pipe(shell, node->right));
			free_exit(shell, execute_pipe(shell, node->right));
        }
        else if (node->right->type == AST_COMMAND)
        {
            debug_print(2047, 8, "Executing right command: %s\n", node->right->cmd->args[0]);
            // exit(execute_simple_command(shell, node->right->cmd));
			free_exit(shell, execute_simple_command(shell, node->right->cmd));
        }
        // exit(1);
		free_exit(shell, 1);
    }

    // 부모 프로세스
    close(pipefd[0]);
    close(pipefd[1]);
    
    // 자식 프로세스들의 종료를 기다림
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    
    debug_print(2047, 8, "Left command status: %d\n", WEXITSTATUS(status1));
    debug_print(2047, 8, "Right command status: %d\n", WEXITSTATUS(status2));
    
    // 마지막 명령어의 종료 상태를 반환
    return (WEXITSTATUS(status2));
}