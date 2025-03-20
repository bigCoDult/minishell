/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 08:46:17 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 14:28:20 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_single_input(t_redirection *current,
	int *last_fd, t_shell *shell)
{
	t_heredoc_entry	*entry;

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
			return (1);
		}
	}
	else if (current->type == REDIR_HEREDOC)
	{
		if (*last_fd != -1)
			close(*last_fd);
		entry = shell->heredoc.entries;
		while (entry)
		{
			if (ft_strcmp(entry->delimiter, current->filename) == 0)
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
		printf("minishell: Heredoc not found for %s\n", current->filename);
		return (1);
	}
	return (0);
}

static int	setup_final_input(int last_fd, t_shell *shell)
{
	if (last_fd != -1)
	{
		if (dup2(last_fd, STDIN_FILENO) == -1)
		{
			close(last_fd);
			restore_io(shell);
			return (1);
		}
		close(last_fd);
	}
	return (0);
}

int	handle_input_redirections(t_shell *shell, t_redirection *redirs)
{
	t_redirection	*current;
	int				last_fd;
	int				error_occurred;

	last_fd = -1;
	error_occurred = 0;
	current = redirs;
	while (current)
	{
		if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
		{
			if (process_single_input(current, &last_fd, shell))
			{
				error_occurred = 1;
				if (last_fd != -1)
					close(last_fd);
				last_fd = -1;
				break ;
			}
		}
		current = current->next;
	}
	if (error_occurred)
		return (1);
	if (last_fd != -1)
		return (setup_final_input(last_fd, shell));
	return (0);
}
