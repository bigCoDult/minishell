/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:08:20 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/04 05:35:34 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void wait_all_children(t_shell *shell, int cmd_count)
{
    int i;
    int status;
    
    i = 0;
    while (i < cmd_count)
    {
        wait(&status);
        if (i == cmd_count - 1)  // 마지막 명령어의 상태 저장
            shell->status.exit_code = WEXITSTATUS(status);
        i++;
    }
}

static int execute_simple_command(t_shell *shell, t_command *cmd)
{
    pid_t pid;
    int status;

    (void)shell;
    pid = fork();
    if (pid == 0)
    {
        // 자식 프로세스
        execvp(cmd->name, cmd->args);
        exit(127);  // 명령어를 찾을 수 없음
    }
    else if (pid > 0)
    {
        // 부모 프로세스
        waitpid(pid, &status, 0);
        return (WEXITSTATUS(status));
    }
    return (1);
}

int execute_commands(t_shell *shell)
{
    t_command *cmd;
    t_command *current;
    int cmd_count;
    int i;
    
    printf("DEBUG: Starting command execution\n");
    
    cmd = shell->commands;
    if (!cmd)
    {
        printf("DEBUG: No commands to execute\n");
        return (0);
    }

    printf("DEBUG: Executing command: %s\n", cmd->name);
    
    // 파이프가 없는 단일 명령어인 경우
    if (!cmd->pipe_next)
    {
        printf("DEBUG: Executing single command\n");
        return (execute_simple_command(shell, cmd));
    }

    // 파이프가 있는 경우
    printf("DEBUG: Executing piped commands\n");
    current = cmd;
    cmd_count = 0;
    while (current)
    {
        cmd_count++;
        current = current->pipe_next;
    }
    
    current = cmd;
    i = 0;
    while (current)
    {
        printf("DEBUG: Executing command %d: %s\n", i + 1, current->name);
        if (execute_piped_command(shell, current, i == 0, 
            i == cmd_count - 1) != 0)
        {
            printf("DEBUG: Command execution failed\n");
            return (1);
        }
        current = current->pipe_next;
        i++;
    }
    
    wait_all_children(shell, cmd_count);
    printf("DEBUG: Command execution completed\n");
    return (0);
}