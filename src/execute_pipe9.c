/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe9.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 07:21:35 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 07:24:32 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_pipe(int pipefd[2], int stderr_backup)
{
	close(pipefd[0]);
	close(pipefd[1]);
	close(stderr_backup);
}

int	setup_pipe_and_execute(
	t_shell *shell, t_ast_node *node, int stderr_backup)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (setup_pipe_in(pipefd))
	{
		close(stderr_backup);
		return (1);
	}
	pid1 = execute_left_child(shell, node, pipefd);
	if (pid1 == -1)
	{
		close_pipe(pipefd, stderr_backup);
		return (1);
	}
	pid2 = execute_right_child(shell, node, pipefd, pid1);
	if (pid2 == -1)
	{
		close_pipe(pipefd, stderr_backup);
		return (1);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	return (wait_for_children(pid1, pid2));
}
