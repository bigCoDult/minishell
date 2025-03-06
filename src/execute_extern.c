/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_extern.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:38:39 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 19:08:40 by yutsong          ###   ########.fr       */
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

static int	handle_signal_termination(t_shell *shell, int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			g_signal = SIGINT;
			shell->status.exit_code = 130;
			return (130);
		}
		else if (WTERMSIG(status) == SIGQUIT)
		{
			g_signal = SIGQUIT;
			shell->status.exit_code = 131;
			return (131);
		}
	}
	return (WEXITSTATUS(status));
}

int	execute_external(t_shell *shell, t_command *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_command_in_child(shell, cmd);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		return (handle_signal_termination(shell, status));
	}
	return (1);
}
