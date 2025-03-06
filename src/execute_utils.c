/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:37:56 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 18:00:12 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd_name)
{
	return (!ft_strcmp(cmd_name, "echo")
		|| !ft_strcmp(cmd_name, "cd")
		|| !ft_strcmp(cmd_name, "pwd")
		|| !ft_strcmp(cmd_name, "export")
		|| !ft_strcmp(cmd_name, "unset")
		|| !ft_strcmp(cmd_name, "env")
		|| !ft_strcmp(cmd_name, "exit"));
}

int	execute_builtin(t_shell *shell, t_command *cmd)
{
	if (!ft_strcmp(cmd->args[0], "echo"))
		return (builtin_echo(cmd->args));
	else if (!ft_strcmp(cmd->args[0], "cd"))
		return (builtin_cd(shell, cmd->args));
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(cmd->args[0], "export"))
		return (builtin_export(shell, cmd->args));
	else if (!ft_strcmp(cmd->args[0], "unset"))
		return (builtin_unset(shell, cmd->args));
	else if (!ft_strcmp(cmd->args[0], "env"))
		return (builtin_env(shell));
	else if (!ft_strcmp(cmd->args[0], "exit"))
		return (builtin_exit(shell, cmd->args));
	return (1);
}

void	wait_all_children(t_shell *shell, int cmd_count)
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_count)
	{
		wait(&status);
		if (i == cmd_count - 1)
			shell->status.exit_code = WEXITSTATUS(status);
		i++;
	}
}
