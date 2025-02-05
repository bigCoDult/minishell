/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_simple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:42:37 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/05 01:54:38 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 단일 명령어 파싱
t_ast_node *parse_simple_command(t_shell *shell, t_token **tokens)
{
    t_ast_node *node;

    printf("\nDEBUG: Starting to parse simple command\n");
    if (!*tokens)
    {
        printf("DEBUG: No tokens to parse\n");
        return (NULL);
    }

    printf("DEBUG: Current token type: %d, value: %s\n", 
           (*tokens)->type, (*tokens)->value);

    if ((*tokens)->type == TOKEN_WORD)
    {
        node = create_ast_node(shell, AST_COMMAND);
        if (!node)
        {
            printf("DEBUG: Failed to create AST node\n");
            return (NULL);
        }
        
        node->cmd = create_command(shell, tokens);
        if (!node->cmd)
        {
            printf("DEBUG: Failed to create command\n");
            return (NULL);
        }
        printf("DEBUG: Successfully created command node: %s\n", node->cmd->name);
    }
    else
    {
        printf("DEBUG: Unexpected token type: %d\n", (*tokens)->type);
        return (NULL);
    }

    return (node);
}