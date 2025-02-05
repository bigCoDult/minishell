/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:41:43 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/05 01:53:33 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// AST 노드 생성
t_ast_node *create_ast_node(t_shell *shell, t_ast_type type)
{
    t_ast_node *node;

    printf("DEBUG: Creating AST node of type: %s\n", 
           type == AST_COMMAND ? "COMMAND" : "PIPE");
    node = shell_malloc(shell, sizeof(t_ast_node));
    if (!node)
    {
        printf("DEBUG: Failed to allocate AST node\n");
        return (NULL);
    }
    
    node->type = type;
    node->cmd = NULL;
    node->left = NULL;
    node->right = NULL;
    printf("DEBUG: AST node created successfully\n");
    
    return (node);
}
