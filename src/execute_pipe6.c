/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe6.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 06:44:39 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 14:31:56 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_direct_path(t_shell *shell, char *cmd_path)
{
	if (access(cmd_path, F_OK) != 0 || access(cmd_path, X_OK) != 0)
	{
		free_exit(shell, 127);
		return (NULL);
	}
	return (cmd_path);
}

char	*handle_path_search(t_shell *shell, char *cmd_name)
{
	char	*path;

	path = find_command_path(shell, cmd_name);
	if (!path)
	{
		free_exit(shell, 127);
		return (NULL);
	}
	return (path);
}

static void	run_command(t_shell *shell, char *path, char **args)
{
	execve(path, args, get_env_array(shell));
	printf("minishell: %s: %s\n", args[0], strerror(errno));
	free_exit(shell, 127);
}

void	execute_external_command(t_shell *shell, t_command *cmd)
{
	char	*path;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		free_exit(shell, 0);
		return ;
	}
	if (ft_strchr(cmd->args[0], '/'))
		path = handle_direct_path(shell, cmd->args[0]);
	else
		path = handle_path_search(shell, cmd->args[0]);
	run_command(shell, path, cmd->args);
}
