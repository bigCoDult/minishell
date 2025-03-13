/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 04:32:17 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 06:34:35 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_pipe_in(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

static pid_t	execute_left_child(t_shell *shell,
	t_ast_node *node, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork (left)");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
		execute_left_command(shell, node, pipefd);
	return (pid);
}

static pid_t	execute_right_child(t_shell *shell, t_ast_node *node,
								int pipefd[2], pid_t pid1)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork (right)");
		close(pipefd[0]);
		close(pipefd[1]);
		kill(pid1, SIGTERM);
		waitpid(pid1, NULL, 0);
		return (-1);
	}
	if (pid == 0)
		execute_right_command(shell, node, pipefd);
	return (pid);
}

static int	wait_for_children(pid_t pid1, pid_t pid2)
{
	int	status1;
	int	status2;

	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	return (1);
}

int	execute_pipe(t_shell *shell, t_ast_node *node)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (!node || !node->left || !node->right)
		return (1);
	if (setup_pipe_in(pipefd))
		return (1);
	pid1 = execute_left_child(shell, node, pipefd);
	if (pid1 == -1)
		return (1);
	pid2 = execute_right_child(shell, node, pipefd, pid1);
	if (pid2 == -1)
		return (1);
	close(pipefd[0]);
	close(pipefd[1]);
	return (wait_for_children(pid1, pid2));
}
