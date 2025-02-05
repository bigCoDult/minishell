/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:31:16 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/05 12:00:38 by yutsong          ###   ########.fr       */
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
    printf("DEBUG: Starting pipe execution\n");
    
    if (!node->left || !node->right)
    {
        printf("DEBUG: Invalid pipe node (missing left or right)\n");
        return (1);
    }

    int cmd_count = 0;
    t_ast_node *current = node;
    
    // 파이프라인의 명령어 개수 세기
    while (current && current->type == AST_PIPE)
    {
        cmd_count++;
        current = current->right;
    }
    if (current)  // 마지막 명령어
        cmd_count++;
    
    printf("DEBUG: Found %d commands in pipeline\n", cmd_count);

    // 파이프라인 실행
    current = node;
    int i = 0;
    while (current)
    {
        t_command *cmd;
        if (current->type == AST_PIPE)
        {
            printf("DEBUG: Executing command %d in pipe\n", i + 1);
            cmd = current->left->cmd;
        }
        else
        {
            printf("DEBUG: Executing final command in pipe\n");
            cmd = current->cmd;
        }

        if (execute_piped_command(shell, cmd) != 0)
        {
            printf("DEBUG: Pipe command execution failed\n");
            return (1);
        }

        if (current->type == AST_PIPE)
            current = current->right;
        else
            current = NULL;
        i++;
    }

    wait_all_children(shell, cmd_count);
    printf("DEBUG: Pipe execution completed\n");
    return (0);
}