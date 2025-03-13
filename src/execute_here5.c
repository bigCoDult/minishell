/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 06:40:00 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 06:41:13 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_heredoc_read(t_shell *shell, t_heredoc_entry *entry, char *temp_file)
{
	int	fd;

	fd = open(temp_file, O_RDONLY);
	if (fd == -1)
	{
		unlink(temp_file);
		shell_free(shell, temp_file);
		return (1);
	}
	entry->fd = fd;
	shell->heredoc.temp_file = temp_file;
	entry->next = shell->heredoc.entries;
	shell->heredoc.entries = entry;
	shell->heredoc.count++;
	return (0);
}

int	get_heredoc_fd(t_shell *shell, char *delimiter)
{
	t_heredoc_entry	*current;

	current = shell->heredoc.entries;
	while (current)
	{
		if (ft_strcmp(current->delimiter, delimiter) == 0)
			return (current->fd);
		current = current->next;
	}
	return (-1);
}
