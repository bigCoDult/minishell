/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 07:24:56 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 07:28:56 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_stderr_backup(void)
{
	int	stderr_backup;

	stderr_backup = dup(STDERR_FILENO);
	if (stderr_backup == -1)
	{
		perror("dup");
		return (-1);
	}
	return (stderr_backup);
}

static int	check_nodes_validity(t_ast_node *node)
{
	if (!node || !node->left || !node->right)
		return (1);
	return (0);
}

static int	handle_redirections(
	t_shell *shell, t_ast_node *node, int stderr_backup)
{
	int	left_failed;

	left_failed = 0;
	if (node->left->type == AST_COMMAND && node->left->cmd->redirs)
		left_failed = check_left_redirections(shell, node);
	if (node->right->type == AST_COMMAND && node->right->cmd->redirs)
		check_right_redirections(shell, node);
	if (left_failed)
	{
		print_left_error(shell, node);
		close(stderr_backup);
		return (1);
	}
	return (0);
}

int	execute_pipe(t_shell *shell, t_ast_node *node)
{
	int	stderr_backup;
	int	result;

	if (check_nodes_validity(node))
		return (1);
	stderr_backup = setup_stderr_backup();
	if (stderr_backup == -1)
		return (1);
	if (handle_redirections(shell, node, stderr_backup))
		return (1);
	result = setup_pipe_and_execute(shell, node, stderr_backup);
	close(stderr_backup);
	return (result);
}
