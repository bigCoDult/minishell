/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 04:36:37 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 08:38:29 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_left_command_io(t_shell *shell, t_command *cmd)
{
	int	heredoc_fd;

	heredoc_fd = find_command_heredoc_fd(shell, cmd);
	if (heredoc_fd != -1)
	{
		// lseek?
		lseek(heredoc_fd, 0, SEEK_SET);
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
	}
	if (setup_redirections(shell, cmd->redirs) != 0)
		free_exit(shell, 1);
}
