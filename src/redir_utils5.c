/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 06:44:08 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 17:57:33 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_and_open_heredoc(
	t_heredoc_entry *entries, const char *delimiter, int *last_fd)
{
	t_heredoc_entry	*entry;

	entry = entries;
	while (entry)
	{
		if (ft_strcmp(entry->delimiter, delimiter) == 0)
		{
			*last_fd = open(entry->temp_file, O_RDONLY);
			if (*last_fd == -1)
			{
				printf("minishell: Failed to open heredoc file\n");
				return (1);
			}
			return (0);
		}
		entry = entry->next;
	}
	printf("minishell: Heredoc not found for %s\n", delimiter);
	return (1);
}

int	process_single_input(t_redirection *current,
	int *last_fd, t_shell *shell)
{
	if (current->type == REDIR_IN)
	{
		if (*last_fd != -1)
			close(*last_fd);
		*last_fd = open(current->filename, O_RDONLY);
		if (*last_fd == -1)
		{
			write(STDERR_FILENO, "minishell: ", 11);
			write(STDERR_FILENO,
				current->filename, ft_strlen(current->filename));
			write(STDERR_FILENO, ": No such file or directory\n", 28);
			shell->status.exit_code = 1;
			return (1);
		}
	}
	else if (current->type == REDIR_HEREDOC)
	{
		if (*last_fd != -1)
			close(*last_fd);
		return (find_and_open_heredoc(
				shell->heredoc.entries, current->filename, last_fd));
	}
	return (0);
}
