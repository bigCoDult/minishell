/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:13:01 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/18 08:11:52 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_heredoc_entries(t_shell *shell)
{
	t_heredoc_entry	*current;
	t_heredoc_entry	*next;

	current = shell->heredoc.entries;
	while (current)
	{
		next = current->next;
		if (current->fd != -1)
		{
			close(current->fd);
		}
		if (current->temp_file)
		{
			unlink(current->temp_file);
			shell_free(shell, current->temp_file);
		}
		shell_free(shell, current->delimiter);
		shell_free(shell, current);
		current = next;
	}
	shell->heredoc.entries = NULL;
	shell->heredoc.count = 0;
}

int	find_command_heredoc_fd(t_shell *shell, t_command *cmd)
{
	t_redirection	*redir;
	t_heredoc_entry	*entry;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			entry = shell->heredoc.entries;
			while (entry)
			{
				if (ft_strcmp(entry->delimiter, redir->filename) == 0)
					return (entry->fd);
				entry = entry->next;
			}
		}
		redir = redir->next;
	}
	return (-1);
}

void	setup_command_heredoc(t_shell *shell, t_command *cmd)
{
	int				heredoc_fd;
	t_redirection	*redir;
	t_heredoc_entry	*entry;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			entry = shell->heredoc.entries;
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
		redir = redir->next;
	}
}
