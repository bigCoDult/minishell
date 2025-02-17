/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_simple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:42:37 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/17 20:22:11 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 단일 명령어 파싱
t_ast_node *parse_simple_command(t_shell *shell, t_token **tokens)
{
    t_ast_node *node;
    t_command *cmd;

    debug_print(0, 7, "DEBUG: [parse_simple_command] Starting\n");
    if (!*tokens)
    {
        debug_print(0, 7, "DEBUG: [parse_simple_command] No tokens to parse\n");
        return (NULL);
    }

    debug_print(0, 7, "DEBUG: [parse_simple_command] Creating AST node\n");
    node = create_ast_node(shell, AST_COMMAND);
    if (!node)
    {
        debug_print(0, 7, "DEBUG: [parse_simple_command] Failed to create AST node\n");
        return (NULL);
    }

    debug_print(0, 7, "DEBUG: [parse_simple_command] Creating command\n");
    cmd = create_command(shell, tokens);
    if (!cmd)
    {
        debug_print(0, 7, "DEBUG: [parse_simple_command] Failed to create command\n");
        return (NULL);
    }

    debug_print(0, 7, "DEBUG: [parse_simple_command] Setting command in AST node\n");
    node->cmd = cmd;

    debug_print(0, 7, "DEBUG: [parse_simple_command] Simple command parsing completed\n");
    return (node);
}