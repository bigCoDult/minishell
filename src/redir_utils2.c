/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 08:36:06 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/21 01:00:21 by yutsong          ###   ########.fr       */
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
