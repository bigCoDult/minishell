/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_extern.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:38:39 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/04 05:13:37 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_external(t_shell *shell, t_command *cmd)
{
    pid_t pid;
    int status;
    char *path;

    printf("DEBUG: execute_external start with cmd: %s\n", cmd->name);
    
    pid = fork();
    if (pid == 0)
    {
        printf("DEBUG: Child process started\n");
        
        // 리다이렉션 설정 (있는 경우)
        if (cmd->redirs)
        {
            printf("DEBUG: Setting up redirections\n");
            setup_redirections(shell, cmd->redirs);
        }
            
        // PATH에서 명령어 찾기
        printf("DEBUG: Searching for command in PATH\n");
        path = find_command_path(shell, cmd->name);
        if (!path)
        {
            printf("minishell: %s: command not found\n", cmd->name);
            exit(127);
        }
        
        printf("DEBUG: Found command path: %s\n", path);
        printf("DEBUG: Executing command with args:\n");
        for (int i = 0; cmd->args[i]; i++)
            printf("DEBUG: arg[%d]: %s\n", i, cmd->args[i]);
        
        execve(path, cmd->args, get_env_array(shell));
        printf("DEBUG: execve failed\n");
        exit(127);
    }
    else if (pid > 0)
    {
        printf("DEBUG: Parent process waiting for child\n");
        waitpid(pid, &status, 0);
        printf("DEBUG: Child process finished with status: %d\n", WEXITSTATUS(status));
        return (WEXITSTATUS(status));
    }
    printf("DEBUG: Fork failed\n");
    return (1);
}