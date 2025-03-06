/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_extern.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:38:39 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 08:42:07 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_command_in_child(t_shell *shell, t_command *cmd)
{
	char	*path;

	if (cmd->redirs)
		setup_redirections(shell, cmd->redirs);
	path = find_command_path(shell, cmd->args[0]);
	if (!path)
	{
		printf("minishell: %s: command not found\n", cmd->args[0]);
		free_exit(shell, 127);
	}
	if (shell->heredoc.entries)
	{
		dup2(shell->heredoc.entries->fd, STDIN_FILENO);
		close(shell->heredoc.entries->fd);
	}
	execve(path, cmd->args, get_env_array(shell));
	free_exit(shell, 127);
}

int	execute_external(t_shell *shell, t_command *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		execute_command_in_child(shell, cmd);
	else if (pid > 0)
	{
		// waitpid?
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	return (1);
}
