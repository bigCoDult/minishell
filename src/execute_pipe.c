/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:31:16 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/10 12:04:28 by yutsong          ###   ########.fr       */
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

int execute_pipe(t_shell *shell, t_ast_node *node)
{
    int pipefd[2];
    pid_t pid1, pid2;
    int status;

    debug_print(1, 1, "\n=== EXECUTE PIPE ===\n");
    
    // 노드 유효성 검사 추가
    if (!node || !node->left || !node->right)
    {
        debug_print(1, 1, "Invalid pipe node structure\n");
        return (1);
    }

    // 명령어 노드 검증 후 디버그 출력
    if (node->left->type == AST_COMMAND && node->left->cmd && node->left->cmd->args)
        debug_print(1, 1, "Left command: %s\n", node->left->cmd->args[0]);
    if (node->right->type == AST_COMMAND && node->right->cmd && node->right->cmd->args)
        debug_print(1, 1, "Right command: %s\n", node->right->cmd->args[0]);
    
    if (pipe(pipefd) == -1)
    {
        debug_print(1, 1, "Pipe creation failed\n");
        return (1);
    }

    // 첫 번째 명령어 실행
    pid1 = fork();
    if (pid1 == -1)
    {
        debug_print(1, 1, "Fork failed for first command\n");
        close(pipefd[0]);
        close(pipefd[1]);
        return (1);
    }
    
    if (pid1 == 0)
    {
        // 자식 프로세스 1
        debug_print(1, 1, "Child 1: Starting execution\n");
        close(pipefd[0]);  // 읽기 끝 닫기
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            debug_print(1, 1, "Child 1: dup2 failed\n");
            exit(1);
        }
        close(pipefd[1]);
        
        // 왼쪽 노드가 파이프인 경우 재귀적으로 실행
        if (node->left->type == AST_PIPE)
        {
            debug_print(1, 1, "Child 1: Found nested pipe, executing recursively\n");
            exit(execute_pipe(shell, node->left));
        }
        else if (node->left->type == AST_COMMAND && node->left->cmd)
        {
            debug_print(1, 1, "Child 1: Executing simple command\n");
            exit(execute_simple_command(shell, node->left->cmd));
        }
        else
        {
            debug_print(1, 1, "Child 1: Invalid command node\n");
            exit(1);
        }
    }

    // 두 번째 명령어 실행
    pid2 = fork();
    if (pid2 == -1)
    {
        debug_print(1, 1, "Fork failed for second command\n");
        close(pipefd[0]);
        close(pipefd[1]);
        return (1);
    }

    if (pid2 == 0)
    {
        // 자식 프로세스 2
        debug_print(1, 1, "Child 2: Starting execution\n");
        close(pipefd[1]);  // 쓰기 끝 닫기
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            debug_print(1, 1, "Child 2: dup2 failed\n");
            exit(1);
        }
        close(pipefd[0]);
        
        // 오른쪽 노드가 파이프인 경우 재귀적으로 실행
        if (node->right->type == AST_PIPE)
        {
            debug_print(1, 1, "Child 2: Found nested pipe, executing recursively\n");
            exit(execute_pipe(shell, node->right));
        }
        else if (node->right->type == AST_COMMAND && node->right->cmd)
        {
            debug_print(1, 1, "Child 2: Executing simple command\n");
            exit(execute_simple_command(shell, node->right->cmd));
        }
        else
        {
            debug_print(1, 1, "Child 2: Invalid command node\n");
            exit(1);
        }
    }

    // 부모 프로세스
    debug_print(1, 1, "Parent: Closing pipe ends\n");
    close(pipefd[0]);
    close(pipefd[1]);

    // 자식 프로세스들의 종료를 기다림
    debug_print(1, 1, "Parent: Waiting for children\n");
    waitpid(pid1, &status, 0);
    debug_print(1, 1, "Parent: First child exited with status: %d\n", WEXITSTATUS(status));
    waitpid(pid2, &status, 0);
    debug_print(1, 1, "Parent: Second child exited with status: %d\n", WEXITSTATUS(status));

    debug_print(1, 1, "=== PIPE EXECUTION COMPLETED ===\n\n");
    return (WEXITSTATUS(status));
}