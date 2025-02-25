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

// 파이프라인 파싱 (오른쪽 재귀 구조)
t_ast_node *parse_pipeline(t_shell *shell, t_token **tokens)
{
    t_ast_node *first_cmd;
    // t_token *start_token;

    debug_print(2047, 8, "\nDEBUG: Starting to parse pipeline\n");
    
    // 첫 번째 명령어 파싱
    first_cmd = parse_simple_command(shell, tokens);
    if (!first_cmd)
    {
        debug_print(2047, 8, "DEBUG: Failed to parse first command\n");
        return (NULL);
    }

    // 파이프가 있는 경우
    if (*tokens && (*tokens)->type == TOKEN_PIPE)
    {
        t_ast_node *pipe_node;
        
        debug_print(2047, 8, "DEBUG: Found pipe token\n");
        *tokens = (*tokens)->next;  // 파이프 토큰 건너뛰기
        
        pipe_node = create_ast_node(shell, AST_PIPE);
        if (!pipe_node)
        {
            debug_print(2047, 8, "DEBUG: Failed to create pipe node\n");
            return (NULL);
        }

        // 왼쪽에 첫 번째 명령어 설정
        pipe_node->left = first_cmd;
        
        // 오른쪽에 나머지 파이프라인 파싱 결과 설정 (재귀)
        pipe_node->right = parse_pipeline(shell, tokens);
        if (!pipe_node->right)
        {
            debug_print(2047, 8, "DEBUG: Failed to parse right pipeline\n");
            return (NULL);
        }

        debug_print(2047, 8, "DEBUG: Successfully created pipe node\n");
        return pipe_node;
    }

    // 파이프가 없는 경우 단일 명령어 반환
    return first_cmd;
}