/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 08:46:17 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/19 13:01:27 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_single_input(t_redirection *current, int *last_fd)
{
	if (current->type == REDIR_IN)
	{
		if (*last_fd != -1)
			close(*last_fd);
		*last_fd = open(current->filename, O_RDONLY);
		if (*last_fd == -1)
		{
			printf(
				"minishell: %s: No such file or directory\n", current->filename);
			return (1);
		}
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

	last_fd = -1;
	current = redirs;
	while (current)
	{
		if (current->type == REDIR_IN)
		{
			if (process_single_input(current, &last_fd))
			{
				restore_io(shell);
				return (1);
			}
		}
		current = current->next;
	}
	return (setup_final_input(last_fd, shell));
}
