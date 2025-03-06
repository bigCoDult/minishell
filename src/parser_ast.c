/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:41:43 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 05:00:13 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*create_ast_node(t_shell *shell, t_ast_type type)
{
	t_ast_node	*node;

	node = shell_malloc(shell, sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->cmd = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}
