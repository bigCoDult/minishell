/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe7.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 06:50:52 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 14:31:36 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_pipe_stdin(int pipefd[2])
{
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2 error in right command");
		close(pipefd[0]);
		return (1);
	}
	close(pipefd[0]);
	return (0);
}

static void	handle_pipe_node(t_shell *shell, t_ast_node *node)
{
	int	ret;

	ret = execute_pipe(shell, node);
	free_exit(shell, ret);
}

static void	handle_command_node(t_shell *shell, t_ast_node *node)
{
	int	ret;

	if (!node->cmd->args || !node->cmd->args[0])
	{
		free_exit(shell, 0);
		return ;
	}
	if (is_builtin(node->cmd->args[0]))
	{
		ret = execute_builtin(shell, node->cmd);
		free_exit(shell, ret);
	}
	else
		execute_external_command(shell, node->cmd);
}

void	execute_right_command(t_shell *shell, t_ast_node *node, int pipefd[2])
{
	if (node->right->type == AST_PIPE)
	{
		if (setup_pipe_stdin(pipefd))
			free_exit(shell, 1);
		handle_pipe_node(shell, node->right);
	}
	else if (node->right->type == AST_COMMAND)
	{
		if (setup_pipe_stdin(pipefd))
			free_exit(shell, 1);
		if (node->right->cmd->redirs
			&& setup_redirections(shell, node->right->cmd->redirs) != 0)
			free_exit(shell, 1);
		handle_command_node(shell, node->right);
	}
	write(STDERR_FILENO, "minishell: right command error\n", 31);
	free_exit(shell, 1);
}
