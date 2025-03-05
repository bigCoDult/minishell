/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:31:16 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/25 11:42:12 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredocs_recursive(t_shell *shell, t_ast_node *node)
{
	t_redirection	*redir;

	if (!node)
		return (0);
	if (node->type == AST_COMMAND)
	{
		redir = node->cmd->redirs;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				if (handle_heredoc(shell, redir->filename) != 0)
					return (1);
			}
			redir = redir->next;
		}
		return (0);
	}
	if (node->type == AST_PIPE)
	{
		if (handle_heredocs_recursive(shell, node->left) != 0)
			return (1);
		return (handle_heredocs_recursive(shell, node->right));
	}
	return (0);
}

void	setup_pipe(t_shell *shell, int pipefd[2], int is_first, int is_last)
{
	if (!is_first)
	{
		dup2(shell->pipe_info.prev_pipe, STDIN_FILENO);
		close(shell->pipe_info.prev_pipe);
	}
	if (!is_last)
		dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
}

int	execute_piped_command(t_shell *shell, t_command *cmd)
{
	pid_t	pid;
	char	*executable_path;
	char	**env_array;

	pid = fork();
	if (pid == 0)
	{
		if (shell->pipe_info.prev_pipe != -1)
		{
			dup2(shell->pipe_info.prev_pipe, STDIN_FILENO);
			close(shell->pipe_info.prev_pipe);
		}
		if (!shell->pipe_info.is_last)
			dup2(shell->pipe_info.pipefd[1], STDOUT_FILENO);
		close(shell->pipe_info.pipefd[0]);
		close(shell->pipe_info.pipefd[1]);
		executable_path = find_command_path(shell, cmd->args[0]);
		if (!executable_path)
			free_exit(shell, 127);
		env_array = get_env_array(shell);
		if (!env_array)
			free_exit(shell, 1);
		execve(executable_path, cmd->args, env_array);
		free_exit(shell, 127);
	}
	if (shell->pipe_info.prev_pipe != -1)
		close(shell->pipe_info.prev_pipe);
	close(shell->pipe_info.pipefd[1]);
	return (pid);
}

static int	handle_all_heredocs(t_shell *shell, t_ast_node *node)
{
	int	original_stdin;
	int	debug_fd;
	int	result;

	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
		return (1);
	debug_fd = dup(STDOUT_FILENO);
	if (debug_fd == -1)
	{
		close(original_stdin);
		return (1);
	}
	shell->heredoc.original_stdin = original_stdin;
	result = handle_heredocs_recursive(shell, node);
	dup2(debug_fd, STDOUT_FILENO);
	close(debug_fd);
	return (result);
}

int	execute_pipe(t_shell *shell, t_ast_node *node)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;
	int		heredoc_fd;
	char	*path;
	int		ret;

	if (!node || !node->left || !node->right)
		return (1);
	if (handle_all_heredocs(shell, node) != 0)
		return (1);
	if (pipe(pipefd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 == 0)
	{
		heredoc_fd = -1;
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		if (node->left->type == AST_COMMAND)
		{
			heredoc_fd = find_command_heredoc_fd(shell, node->left->cmd);
			if (heredoc_fd != -1)
			{
				lseek(heredoc_fd, 0, SEEK_SET);
				dup2(heredoc_fd, STDIN_FILENO);
				close(heredoc_fd);
			}
			if (setup_redirections(shell, node->left->cmd->redirs) != 0)
				free_exit(shell, 1);
			if (is_builtin(node->left->cmd->args[0]))
			{
				ret = execute_builtin(shell, node->left->cmd);
				free_exit(shell, ret);
			}
			else
			{
				path = find_command_path(shell, node->left->cmd->args[0]);
				if (!path)
					free_exit(shell, 127);
				execve(path, node->left->cmd->args, get_env_array(shell));
				free_exit(shell, 127);
			}
		}
		free_exit(shell, 1);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		if (node->right->type == AST_PIPE)
		{
			ret = execute_pipe(shell, node->right);
			exit(ret);
		}
		else if (node->right->type == AST_COMMAND)
		{
			heredoc_fd = find_command_heredoc_fd(shell, node->right->cmd);
			if (heredoc_fd != -1)
			{
				lseek(heredoc_fd, 0, SEEK_SET);
				dup2(heredoc_fd, STDIN_FILENO);
				close(heredoc_fd);
			}
			if (setup_redirections(shell, node->right->cmd->redirs) != 0)
				free_exit(shell, 1);
			if (is_builtin(node->right->cmd->args[0]))
			{
				ret = execute_builtin(shell, node->right->cmd);
				free_exit(shell, ret);
			}
			else
			{
				path = find_command_path(shell, node->right->cmd->args[0]);
				if (!path)
					free_exit(shell, 127);
				execve(path, node->right->cmd->args, get_env_array(shell));
				free_exit(shell, 127);
			}
		}
		free_exit(shell, 1);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	return (WEXITSTATUS(status2));
}
