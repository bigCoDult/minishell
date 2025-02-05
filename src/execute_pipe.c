/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:31:16 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/05 02:19:14 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void setup_pipe(t_shell *shell, int pipefd[2], int is_first, int is_last)
{
    if (!is_first)  // 첫 번째 명령어가 아니면
    {
        dup2(shell->pipe.read_fd, STDIN_FILENO);  // 이전 파이프의 읽기를 표준 입력으로
        close(shell->pipe.read_fd);
    }
    
    if (!is_last)  // 마지막 명령어가 아니면
    {
        dup2(pipefd[1], STDOUT_FILENO);  // 현재 파이프의 쓰기를 표준 출력으로
    }
    
    close(pipefd[0]);
    close(pipefd[1]);
}

int execute_piped_command(t_shell *shell, t_command *cmd, int is_first, int is_last)
{
    pid_t pid;
    int pipefd[2];
    
    if (!is_last && pipe(pipefd) < 0)
        return (1);
    
    pid = fork();
    if (pid == 0)
    {
        // 자식 프로세스
        setup_pipe(shell, pipefd, is_first, is_last);
        execvp(cmd->name, cmd->args);
        exit(127);  // 명령어를 찾을 수 없음
    }
    else if (pid < 0)
    {
        return (1);
    }
    
    // 부모 프로세스
    if (!is_first)
        close(shell->pipe.read_fd);
    if (!is_last)
    {
        close(pipefd[1]);
        shell->pipe.read_fd = pipefd[0];  // 다음 명령어를 위해 읽기 fd 저장
    }
    
    return (0);
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

        if (execute_piped_command(shell, cmd, i == 0, i == cmd_count - 1) != 0)
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