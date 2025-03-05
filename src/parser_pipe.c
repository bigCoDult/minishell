/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:43:05 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/25 11:42:12 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*parse_pipeline(t_shell *shell, t_token **tokens)
{
	t_ast_node	*first_cmd;
	t_ast_node	*pipe_node;

	debug_print(2047, 8, "\nDEBUG: Starting to parse pipeline\n");
	first_cmd = parse_simple_command(shell, tokens);
	if (!first_cmd)
	{
		debug_print(2047, 8, "DEBUG: Failed to parse first command\n");
		return (NULL);
	}
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		debug_print(2047, 8, "DEBUG: Found pipe token\n");
		*tokens = (*tokens)->next;
		pipe_node = create_ast_node(shell, AST_PIPE);
		if (!pipe_node)
		{
			debug_print(2047, 8, "DEBUG: Failed to create pipe node\n");
			return (NULL);
		}
		pipe_node->left = first_cmd;
		pipe_node->right = parse_pipeline(shell, tokens);
		if (!pipe_node->right)
		{
			debug_print(2047, 8, "DEBUG: Failed to parse right pipeline\n");
			return (NULL);
		}
		debug_print(2047, 8, "DEBUG: Successfully created pipe node\n");
		return (pipe_node);
	}
	return (first_cmd);
}
