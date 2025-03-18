/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 04:36:37 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/18 18:36:57 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_left_sub(t_redirection *redir, t_heredoc_entry *entry)
{
	int	heredoc_fd;

	while (entry)
	{
		if (ft_strcmp(entry->delimiter, redir->filename) == 0)
		{
			if (entry->fd != -1)
				close(entry->fd);
			heredoc_fd = open(entry->temp_file, O_RDONLY);
			if (heredoc_fd != -1)
			{
				dup2(heredoc_fd, STDIN_FILENO);
				close(heredoc_fd);
			}
			break ;
		}
		entry = entry->next;
	}
}

void	setup_left_command_io(t_shell *shell, t_command *cmd)
{
	t_redirection	*redir;
	t_heredoc_entry	*entry;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			entry = shell->heredoc.entries;
			setup_left_sub(redir, entry);
		}
		redir = redir->next;
	}
	if (setup_redirections(shell, cmd->redirs) != 0)
		free_exit(shell, 1);
}
