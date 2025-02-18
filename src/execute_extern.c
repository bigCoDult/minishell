/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_extern.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:38:39 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/18 22:25:55 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_external(t_shell *shell, t_command *cmd)
{
    pid_t pid;
    int status;
    char *path;

    debug_print(2047, 7, "DEBUG: execute_external start with cmd: %s\n", cmd->args[0]);
    
    pid = fork();
    if (pid == 0)
    {
        debug_print(2047, 7, "DEBUG: Child process started\n");
        
        // 리다이렉션 설정 (있는 경우)
        if (cmd->redirs)
        {
            debug_print(2047, 10, "DEBUG: Setting up redirections\n");
            setup_redirections(shell, cmd->redirs);
        }
            
        // PATH에서 명령어 찾기
        debug_print(2047, 6, "DEBUG: Searching for command in PATH\n");
        path = find_command_path(shell, cmd->args[0]);
        if (!path)
        {
            printf("minishell: %s: command not found\n", cmd->args[0]);
            // exit(127);
			free_exit(shell, 127);
        }
        
        debug_print(2047, 6, "DEBUG: Found command path: %s\n", path);
        debug_print(2047, 7, "DEBUG: Executing command with args:\n");
        for (int i = 0; cmd->args[i]; i++)
            debug_print(2047, 7, "DEBUG: arg[%d]: %s\n", i, cmd->args[i]);
        
        // 히어독이 있었다면 표준 입력 설정
        if (shell->heredoc.fd != -1)
        {
            dup2(shell->heredoc.fd, STDIN_FILENO);
            close(shell->heredoc.fd);
        }
        
        execve(path, cmd->args, get_env_array(shell));
        debug_print(2047, 7, "DEBUG: execve failed\n");
        // exit(127);
		free_exit(shell, 127);
    }
    else if (pid > 0)
    {
        debug_print(2047, 7, "DEBUG: Parent process waiting for child\n");
        waitpid(pid, &status, 0);
        debug_print(2047, 7, "DEBUG: Child process finished with status: %d\n", WEXITSTATUS(status));
        return (WEXITSTATUS(status));
    }
    debug_print(2047, 7, "DEBUG: Fork failed\n");
    return (1);
}