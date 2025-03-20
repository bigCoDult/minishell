/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 06:12:15 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 06:12:33 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	verify_command(t_shell *shell, t_command *cmd)
{
	char	*path;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (is_builtin(cmd->args[0]))
		return ;
	if (ft_strchr(cmd->args[0], '/'))
	{
		if (access(cmd->args[0], F_OK) != 0)
			printf("minishell: %s: Nos such file or directory\n", cmd->args[0]);
		else if (access(cmd->args[0], X_OK) != 0)
			printf("minishell: %s: Permission denied\n", cmd->args[0]);
		return ;
	}
	path = find_command_path(shell, cmd->args[0]);
	if (!path)
		printf("minishell: %s: command not found\n", cmd->args[0]);
	else
		shell_free(shell, path);
}

static void	verify_commands_recursive(t_shell *shell, t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == AST_COMMAND)
		verify_command(shell, node->cmd);
	else if (node->type == AST_PIPE)
	{
		verify_commands_recursive(shell, node->left);
		verify_commands_recursive(shell, node->right);
	}
}

int	execute_ast(t_shell *shell, t_ast_node *node)
{
	if (!node)
		return (0);
	if (node->type == AST_PIPE)
		verify_commands_recursive(shell, node);
	if (node->type == AST_COMMAND)
	{
		if (node->cmd->redirs)
		{
			if (process_redirection(shell, node->cmd->redirs) != 0)
				return (1);
		}
		return (execute_simple_command(shell, node->cmd));
	}
	else if (node->type == AST_PIPE)
		return (execute_pipe(shell, node));
	return (1);
}
