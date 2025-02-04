/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:37:56 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/04 04:38:09 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 빌트인 명령어 체크
int is_builtin(char *cmd_name)
{
    return (!strcmp(cmd_name, "echo") ||
            !strcmp(cmd_name, "cd") ||
            !strcmp(cmd_name, "pwd") ||
            !strcmp(cmd_name, "export") ||
            !strcmp(cmd_name, "unset") ||
            !strcmp(cmd_name, "env") ||
            !strcmp(cmd_name, "exit"));
}

// 빌트인 명령어 실행
int execute_builtin(t_shell *shell, t_command *cmd)
{
    if (!strcmp(cmd->name, "echo"))
        return (builtin_echo(cmd->args));
    else if (!strcmp(cmd->name, "cd"))
        return (builtin_cd(shell, cmd->args));
    else if (!strcmp(cmd->name, "pwd"))
        return (builtin_pwd());
    else if (!strcmp(cmd->name, "export"))
        return (builtin_export(shell, cmd->args));
    else if (!strcmp(cmd->name, "unset"))
        return (builtin_unset(shell, cmd->args));
    else if (!strcmp(cmd->name, "env"))
        return (builtin_env(shell));
    else if (!strcmp(cmd->name, "exit"))
        return (builtin_exit(shell, cmd->args));
    return (1);
}