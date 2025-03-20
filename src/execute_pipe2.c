/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 04:32:17 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 14:38:13 by yutsong          ###   ########.fr       */
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
	int				pipefd[2];
	pid_t			pid1;
	pid_t			pid2;
	int				status1;
	int				check_redir_result;
	int				left_failed;
	int				stderr_backup;
	int				dev_null;
	int				file_fd;
	t_redirection	*redir;

	left_failed = 0;
	if (!node || !node->left || !node->right)
		return (1);
	stderr_backup = dup(STDERR_FILENO);
	if (stderr_backup == -1)
	{
		perror("dup");
		return (1);
	}
	if (node->left->type == AST_COMMAND && node->left->cmd->redirs)
	{
		check_redir_result = fork();
		if (check_redir_result == 0)
		{
			dev_null = open("/dev/null", O_WRONLY);
			if (dev_null != -1)
			{
				dup2(dev_null, STDERR_FILENO);
				close(dev_null);
			}
			if (setup_redirections(shell, node->left->cmd->redirs) != 0)
				free_exit(shell, 1);
			free_exit(shell, 0);
		}
		waitpid(check_redir_result, &status1, 0);
		if (WIFEXITED(status1) && WEXITSTATUS(status1) != 0)
			left_failed = 1;
	}
	if (node->right->type == AST_COMMAND && node->right->cmd->redirs)
	{
		check_redir_result = fork();
		if (check_redir_result == 0)
		{
			redir = node->right->cmd->redirs;
			while (redir)
			{
				if (redir->type == REDIR_IN)
				{
					file_fd = open(redir->filename, O_RDONLY);
					if (file_fd == -1)
					{
						write(STDERR_FILENO, "minishell: ", 11);
						write(STDERR_FILENO, redir->filename, ft_strlen(redir->filename));
						write(STDERR_FILENO, ": No such file or directory\n", 28);
					}
					else
						close(file_fd);
				}
				redir = redir->next;
			}
			free_exit(shell, 0);
		}
		waitpid(check_redir_result, NULL, 0);
	}
	if (left_failed)
	{
		check_redir_result = fork();
		if (check_redir_result == 0)
		{
			setup_redirections(shell, node->left->cmd->redirs);
			free_exit(shell, 1);
		}
		waitpid(check_redir_result, NULL, 0);
		close(stderr_backup);
		return (1);
	}
	if (setup_pipe_in(pipefd))
	{
		close(stderr_backup);
		return (1);
	}
	pid1 = execute_left_child(shell, node, pipefd);
	if (pid1 == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		close(stderr_backup);
		return (1);
	}
	pid2 = execute_right_child(shell, node, pipefd, pid1);
	if (pid2 == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		close(stderr_backup);
		return (1);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	close(stderr_backup);
	return (wait_for_children(pid1, pid2));
}
