/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:43:05 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 18:03:05 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_pipe_error(t_shell *shell, t_token **tokens)
{
	if (!*tokens || (*tokens)->type == TOKEN_PIPE)
	{
		write(STDERR_FILENO,
			"minishell: syntax error near unexpected token '|'\n", 51);
		shell->status.exit_code = 1;
		return (1);
	}
	return (0);
}

static t_ast_node	*create_pipe_node(t_shell *shell, t_ast_node *left_cmd,
									t_token **tokens)
{
	t_ast_node	*pipe_node;
	t_ast_node	*right_cmd;

	pipe_node = create_ast_node(shell, AST_PIPE);
	if (!pipe_node)
		return (NULL);
	pipe_node->left = left_cmd;
	right_cmd = parse_pipeline(shell, tokens);
	if (!right_cmd)
		return (NULL);
	pipe_node->right = right_cmd;
	return (pipe_node);
}

t_ast_node	*parse_pipeline(t_shell *shell, t_token **tokens)
{
	t_ast_node	*first_cmd;

	first_cmd = parse_simple_command(shell, tokens);
	if (!first_cmd)
		return (NULL);
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		if (handle_pipe_error(shell, tokens))
			return (NULL);
		return (create_pipe_node(shell, first_cmd, tokens));
	}
	return (first_cmd);
}
