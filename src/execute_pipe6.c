/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe6.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 06:44:39 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/19 05:19:12 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_direct_path(t_shell *shell, char *cmd_path)
{
	if (access(cmd_path, F_OK) != 0)
	{
		printf("minishell: %s: No such file or directory\n", cmd_path);
		free_exit(shell, 127);
	}
	else if (access(cmd_path, X_OK) != 0)
	{
		printf("minishell: %s: Permission denied\n", cmd_path);
		free_exit(shell, 126);
	}
	return (cmd_path);
}

static char	*handle_path_search(t_shell *shell, char *cmd_name)
{
	char	*path;

	path = find_command_path(shell, cmd_name);
	if (!path)
	{
		printf("minishell: %s: command not found\n", cmd_name);
		free_exit(shell, 127);
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

	if (ft_strchr(cmd->args[0], '/'))
		path = handle_direct_path(shell, cmd->args[0]);
	else
		path = handle_path_search(shell, cmd->args[0]);
	run_command(shell, path, cmd->args);
}
