/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 05:57:25 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/18 08:02:35 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_heredoc_fds(t_shell *shell)
{
	t_heredoc_entry	*entry;

	if (!shell->heredoc.entries)
		return ;
	entry = shell->heredoc.entries;
	while (entry)
	{
		if (entry->fd != -1)
		{
			close(entry->fd);
			entry->fd = -1;
		}
		entry = entry->next;
	}
}

static void	restore_original_stdin(t_shell *shell)
{
	if (shell->heredoc.original_stdin != -1)
	{
		dup2(shell->heredoc.original_stdin, STDIN_FILENO);
		close(shell->heredoc.original_stdin);
		shell->heredoc.original_stdin = -1;
	}
}

static void	cleanup_temp_files(t_shell *shell)
{
	t_heredoc_entry	*entry;

	entry = shell->heredoc.entries;
	while (entry)
	{
		if (entry->temp_file)
		{
			unlink(entry->temp_file);
			shell_free(shell, entry->temp_file);
			entry->temp_file = NULL;
		}
		entry = entry->next;
	}
}

void	cleanup_heredoc(t_shell *shell)
{
	close_heredoc_fds(shell);
	restore_original_stdin(shell);
	cleanup_temp_files(shell);
}
