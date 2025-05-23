/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_simple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:42:37 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 04:58:45 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*parse_simple_command(t_shell *shell, t_token **tokens)
{
	t_ast_node	*node;
	t_command	*cmd;

	if (!*tokens)
		return (NULL);
	node = create_ast_node(shell, AST_COMMAND);
	if (!node)
		return (NULL);
	cmd = create_command(shell, tokens);
	if (!cmd)
		return (NULL);
	node->cmd = cmd;
	return (node);
}
