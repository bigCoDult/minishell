/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_extern2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 07:29:32 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 07:33:19 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	process_child_status(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->status.exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->status.exit_code = 128 + WTERMSIG(status);
	else
		shell->status.exit_code = 1;
	handle_signal_termination(shell, status);
}

static int	wait_for_child(t_shell *shell, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	process_child_status(shell, status);
	return (shell->status.exit_code);
}

int	execute_external(t_shell *shell, t_command *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		setup_child_signals();
		execute_command_in_child(shell, cmd);
	}
	else if (pid > 0)
		return (wait_for_child(shell, pid));
	return (1);
}
