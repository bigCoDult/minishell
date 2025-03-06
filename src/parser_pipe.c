/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:43:05 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 04:59:50 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*parse_pipeline(t_shell *shell, t_token **tokens)
{
	t_ast_node	*first_cmd;
	t_ast_node	*pipe_node;

	first_cmd = parse_simple_command(shell, tokens);
	if (!first_cmd)
		return (NULL);
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		pipe_node = create_ast_node(shell, AST_PIPE);
		if (!pipe_node)
			return (NULL);
		pipe_node->left = first_cmd;
		pipe_node->right = parse_pipeline(shell, tokens);
		if (!pipe_node->right)
			return (NULL);
		return (pipe_node);
	}
	return (first_cmd);
}
