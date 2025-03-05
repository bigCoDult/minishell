/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:37:56 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/05 12:11:36 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd_name)
{
	return (!strcmp(cmd_name, "echo")
		|| !strcmp(cmd_name, "cd")
		|| !strcmp(cmd_name, "pwd")
		|| !strcmp(cmd_name, "export")
		|| !strcmp(cmd_name, "unset")
		|| !strcmp(cmd_name, "env")
		|| !strcmp(cmd_name, "exit"));
}

int	execute_builtin(t_shell *shell, t_command *cmd)
{
	if (!strcmp(cmd->args[0], "echo"))
		return (builtin_echo(cmd->args));
	else if (!strcmp(cmd->args[0], "cd"))
		return (builtin_cd(shell, cmd->args));
	else if (!strcmp(cmd->args[0], "pwd"))
		return (builtin_pwd());
	else if (!strcmp(cmd->args[0], "export"))
		return (builtin_export(shell, cmd->args));
	else if (!strcmp(cmd->args[0], "unset"))
		return (builtin_unset(shell, cmd->args));
	else if (!strcmp(cmd->args[0], "env"))
		return (builtin_env(shell));
	else if (!strcmp(cmd->args[0], "exit"))
		return (builtin_exit(shell, cmd->args));
	return (1);
}
