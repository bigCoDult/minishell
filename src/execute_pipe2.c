/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 04:32:17 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 05:55:58 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_left_command(t_shell *shell, t_ast_node *node, int pipefd[2])
{
	int	ret;

	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2 error in left command");
		close(pipefd[1]);
		free_exit(shell, 1);
	}
	close(pipefd[1]);
	if (node->left->type == AST_COMMAND)
	{
		setup_left_command_io(shell, node->left->cmd);
		if (is_builtin(node->left->cmd->args[0]))
		{
			ret = execute_builtin(shell, node->left->cmd);
			free_exit(shell, ret);
		}
		else
			execute_external_command(shell, node->left->cmd);
	}
	write(STDERR_FILENO, "minishell: left command error\n", 30);
	free_exit(shell, 1);
}

void	execute_external_command(t_shell *shell, t_command *cmd)
{
	char	*path;

	if (ft_strchr(cmd->args[0], '/'))
	{
		if (access(cmd->args[0], F_OK) != 0)
		{
			print_error("minishell: %s: No such file or directory\n", cmd->args[0]);
			free_exit(shell, 127);
		}
		else if (access(cmd->args[0], X_OK) != 0)
		{
			print_error("minishell: %s: Permission denied\n", cmd->args[0]);
			free_exit(shell, 126);
		}
		path = cmd->args[0];
	}
	else
	{
		path = find_command_path(shell, cmd->args[0]);
		if (!path)
		{
			print_error("minishell: %s: command not found\n", cmd->args[0]);
			free_exit(shell, 127);
		}
	}
	execve(path, cmd->args, get_env_array(shell));
	print_error("minishell: %s: %s\n", cmd->args[0], strerror(errno));
	free_exit(shell, 127);
}

void	execute_right_command(t_shell *shell, t_ast_node *node, int pipefd[2])
{
	int	ret;

	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2 error in right command");
		close(pipefd[0]);
		free_exit(shell, 1);
	}
	close(pipefd[0]);
	if (node->right->type == AST_PIPE)
	{
		ret = execute_pipe(shell, node->right);
		free_exit(shell, ret);
	}
	else if (node->right->type == AST_COMMAND)
	{
		setup_right_command_io(shell, node->right->cmd);
		if (is_builtin(node->right->cmd->args[0]))
		{
			ret = execute_builtin(shell, node->right->cmd);
			free_exit(shell, ret);
		}
		else
			execute_external_command(shell, node->right->cmd);
	}
	write(STDERR_FILENO, "minishell: right command error\n", 31);
	free_exit(shell, 1);
}

void	setup_right_command_io(t_shell *shell, t_command *cmd)
{
	int	heredoc_fd;

	heredoc_fd = find_command_heredoc_fd(shell, cmd);
	if (heredoc_fd != -1)
	{
		close(heredoc_fd);
		heredoc_fd = open(shell->heredoc.temp_file, O_RDONLY);
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
	}
	if (setup_redirections(shell, cmd->redirs) != 0)
		free_exit(shell, 1);
}

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
