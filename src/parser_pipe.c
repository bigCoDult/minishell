/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:43:05 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/05 01:55:14 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 파이프라인 파싱
t_ast_node *parse_pipeline(t_shell *shell, t_token **tokens)
{
    t_ast_node *left;
    t_ast_node *pipe_node;

    printf("\nDEBUG: Starting to parse pipeline\n");
    left = parse_simple_command(shell, tokens);
    if (!left)
    {
        printf("DEBUG: Failed to parse left command\n");
        return (NULL);
    }

    // 파이프가 있는 경우
    while (*tokens && (*tokens)->type == TOKEN_PIPE)
    {
        t_ast_node *right;
        
        printf("DEBUG: Found pipe token\n");
        *tokens = (*tokens)->next;  // 파이프 토큰 건너뛰기
        
        pipe_node = create_ast_node(shell, AST_PIPE);
        if (!pipe_node)
        {
            printf("DEBUG: Failed to create pipe node\n");
            return (NULL);
        }

        right = parse_simple_command(shell, tokens);
        if (!right)
        {
            printf("DEBUG: Failed to parse right command\n");
            return (NULL);
        }

        pipe_node->left = left;
        pipe_node->right = right;
        left = pipe_node;
        printf("DEBUG: Successfully created pipe node\n");
    }

    return left;
}