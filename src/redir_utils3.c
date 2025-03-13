/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 08:46:17 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 08:50:45 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			if (last_fd != -1)
				close(last_fd);
			last_fd = open(current->filename, O_RDONLY);
			if (last_fd == -1)
			{
				print_error("minishell: %s: No such file or directory\n",
					current->filename);
				restore_io(shell);
				return (1);
			}
		}
		current = current->next;
	}
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
