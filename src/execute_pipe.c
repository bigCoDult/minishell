/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:31:16 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 04:38:38 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredocs_recursive(t_shell *shell, t_ast_node *node)
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

void	execute_in_pipe(t_shell *shell, t_command *cmd)
{
	char	*executable_path;
	char	**env_array;

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

int	execute_piped_command(t_shell *shell, t_command *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execute_in_pipe(shell, cmd);
	if (shell->pipe_info.prev_pipe != -1)
		close(shell->pipe_info.prev_pipe);
	close(shell->pipe_info.pipefd[1]);
	return (pid);
}

int	handle_all_heredocs(t_shell *shell, t_ast_node *node)
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
