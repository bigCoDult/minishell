/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 08:17:00 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 14:34:29 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_null_output(int *stdout_backup)
{
	int	devnull_fd;

	*stdout_backup = dup(STDOUT_FILENO);
	if (*stdout_backup == -1)
		return (1);
	devnull_fd = open("/dev/null", O_WRONLY);
	if (devnull_fd == -1)
	{
		close(*stdout_backup);
		return (1);
	}
	dup2(devnull_fd, STDOUT_FILENO);
	close(devnull_fd);
	return (0);
}

static int	processing_heredocs(t_shell *shell, int stdout_backup)
{
	int	heredoc_result;

	heredoc_result = handle_all_heredocs(shell, shell->ast_root);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdout_backup);
	if (heredoc_result != 0)
	{
		restore_io(shell);
		return (1);
	}
	return (0);
}

static int	execute_and_cleanup(t_shell *shell)
{
	int	result;

	setup_signals_executing();
	result = execute_ast(shell, shell->ast_root);
	restore_io(shell);
	setup_signals_interactive();
	return (result);
}

int	execute_commands(t_shell *shell)
{
	int	stdout_backup;
	int	result;

	if (g_signal == 130 || g_signal == 131)
	{
		shell->status.exit_code = g_signal;
		g_signal = 0;
	}
	if (!shell->ast_root)
		return (0);
	if (g_signal == 130 || g_signal == 131)
	{
		shell->status.exit_code = g_signal;
		g_signal = 0;
	}
	if (shell->heredoc.original_stdin != -1 || shell->original_stdout != -1)
		restore_io(shell);
	if (g_signal == 130 || g_signal == 131)
	{
		shell->status.exit_code = g_signal;
		g_signal = 0;
	}
	if (setup_null_output(&stdout_backup))
		return (1);
	if (g_signal == 130 || g_signal == 131)
	{
		shell->status.exit_code = g_signal;
		g_signal = 0;
		close(stdout_backup);
		return (0);
	}
	if (processing_heredocs(shell, stdout_backup))
		return (1);
	if (g_signal == 130 || g_signal == 131)
	{
		shell->status.exit_code = g_signal;
		g_signal = 0;
	}
	result = execute_and_cleanup(shell);
	if (g_signal == 130 || g_signal == 131)
	{
		shell->status.exit_code = g_signal;
		g_signal = 0;
	}
	return (result);
}
