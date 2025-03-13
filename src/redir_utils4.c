/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 08:47:31 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 08:48:23 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	handle_output_redirections(t_shell *shell, t_redirection *redirs)
{
	t_redirection	*current;
	int				last_fd;

	last_fd = -1;
	current = redirs;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			if (last_fd != -1)
				close(last_fd);
			last_fd = open_output_file(current);
			if (last_fd == -1)
			{
				print_error("minishell: %s: Permission denied\n",
					current->filename);
				restore_io(shell);
				return (1);
			}
		}
		current = current->next;
	}
	if (last_fd != -1)
	{
		if (dup2(last_fd, STDOUT_FILENO) == -1)
		{
			close(last_fd);
			restore_io(shell);
			return (1);
		}
		close(last_fd);
	}
	return (0);
}