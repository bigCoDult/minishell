/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 08:36:06 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 14:28:42 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	backup_original_fds(t_shell *shell)
{
	if (shell->heredoc.original_stdin == -1)
		shell->heredoc.original_stdin = dup(STDIN_FILENO);
	if (shell->original_stdout == -1)
		shell->original_stdout = dup(STDOUT_FILENO);
	if (shell->original_stderr == -1)
		shell->original_stderr = dup(STDERR_FILENO);
	if (shell->heredoc.original_stdin == -1
		|| shell->original_stdout == -1
		|| shell->original_stderr == -1)
		return (1);
	return (0);
}

void	cleanup_backup_fds(t_shell *shell)
{
	if (shell->heredoc.original_stdin != -1)
	{
		close(shell->heredoc.original_stdin);
		shell->heredoc.original_stdin = -1;
	}
	if (shell->original_stdout != -1)
	{
		close(shell->original_stdout);
		shell->original_stdout = -1;
	}
	if (shell->original_stderr != -1)
	{
		close(shell->original_stderr);
		shell->original_stderr = -1;
	}
}

int	open_output_file(t_redirection *redir)
{
	int	fd;

	if (redir->type == REDIR_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

int	setup_redirections(t_shell *shell, t_redirection *redirs)
{
	int	has_input_error;

	if (!redirs)
		return (0);
	if (backup_original_fds(shell))
	{
		cleanup_backup_fds(shell);
		return (1);
	}
	if (handle_output_redirections(shell, redirs))
		return (1);
	has_input_error = handle_input_redirections(shell, redirs);
	return (has_input_error);
}
