/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 08:46:17 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 06:44:48 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	process_input_redirections(
	t_redirection *current, int *last_fd, t_shell *shell)
{
	while (current)
	{
		if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
		{
			if (process_single_input(current, last_fd, shell))
			{
				if (*last_fd != -1)
					close(*last_fd);
				*last_fd = -1;
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}

int	handle_input_redirections(t_shell *shell, t_redirection *redirs)
{
	int	last_fd;
	int	error_occurred;

	last_fd = -1;
	error_occurred = process_input_redirections(redirs, &last_fd, shell);
	if (error_occurred)
		return (1);
	if (last_fd != -1)
		return (setup_final_input(last_fd, shell));
	return (0);
}
