/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:31:16 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/15 16:42:23 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
            exit(127);

        char **env_array = get_env_array(shell);
        if (!env_array)
            exit(1);

        execve(executable_path, cmd->args, env_array);
        exit(127);
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
    t_redirection *left_redirs = NULL;
    t_redirection *right_redirs = NULL;
    int heredoc_count = 0;
    int original_stdin;

    debug_print(0, 9, "=== HANDLING ALL HEREDOCS ===\n");
    
    // 현재 표준 입력 저장
    original_stdin = dup(STDIN_FILENO);
    if (original_stdin == -1)
        return (1);

    shell->heredoc.original_stdin = original_stdin;  // 쉘 구조체에 저장

    // 왼쪽 명령어의 히어독 찾기
    if (node->left && node->left->type == AST_COMMAND && node->left->cmd)
        left_redirs = node->left->cmd->redirs;
    
    // 오른쪽 명령어의 히어독 찾기
    if (node->right && node->right->type == AST_COMMAND && node->right->cmd)
        right_redirs = node->right->cmd->redirs;

    // 왼쪽 히어독 처리
    while (left_redirs)
    {
        if (left_redirs->type == REDIR_HEREDOC)
        {
            debug_print(0, 9, "Processing left heredoc: %s\n", left_redirs->filename);
            if (handle_heredoc(shell, left_redirs->filename) != 0)
                return (1);
            heredoc_count++;
            
            // 각 히어독 처리 후 표준 입력 복원
            if (dup2(original_stdin, STDIN_FILENO) == -1)
                return (1);
        }
        left_redirs = left_redirs->next;
    }

    // 오른쪽 히어독 처리
    while (right_redirs)
    {
        if (right_redirs->type == REDIR_HEREDOC)
        {
            debug_print(0, 9, "Processing right heredoc: %s\n", right_redirs->filename);
            if (handle_heredoc(shell, right_redirs->filename) != 0)
                return (1);
            heredoc_count++;
            
            // 각 히어독 처리 후 표준 입력 복원
            if (dup2(original_stdin, STDIN_FILENO) == -1)
                return (1);
        }
        right_redirs = right_redirs->next;
    }

    debug_print(0, 9, "Processed %d heredocs\n", heredoc_count);
    
    // original_stdin은 닫지 않고 shell 구조체에 저장된 상태로 유지
    return (0);
}

int execute_pipe(t_shell *shell, t_ast_node *node)
{
    int pipefd[2];
    pid_t pid1, pid2;
    int status1, status2;

    debug_print(0, 8, "\n=== EXECUTE PIPE ===\n");
    
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
            exit(1);
        close(pipefd[1]);
        
        if (node->left->type == AST_PIPE)
            exit(execute_pipe(shell, node->left));
        else if (node->left->type == AST_COMMAND)
        {
            debug_print(0, 8, "Executing left command: %s\n", node->left->cmd->args[0]);
            exit(execute_simple_command(shell, node->left->cmd));
        }
        exit(1);
    }

    // 두 번째 명령어 실행 (오른쪽 노드)
    pid2 = fork();
    if (pid2 == 0)
    {
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
            exit(1);
        close(pipefd[0]);
        
        if (node->right->type == AST_PIPE)
        {
            debug_print(0, 8, "Executing right pipe\n");
            exit(execute_pipe(shell, node->right));
        }
        else if (node->right->type == AST_COMMAND)
        {
            debug_print(0, 8, "Executing right command: %s\n", node->right->cmd->args[0]);
            exit(execute_simple_command(shell, node->right->cmd));
        }
        exit(1);
    }

    // 부모 프로세스
    close(pipefd[0]);
    close(pipefd[1]);
    
    // 자식 프로세스들의 종료를 기다림
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    
    debug_print(0, 8, "Left command status: %d\n", WEXITSTATUS(status1));
    debug_print(0, 8, "Right command status: %d\n", WEXITSTATUS(status2));
    
    // 마지막 명령어의 종료 상태를 반환
    return (WEXITSTATUS(status2));
}