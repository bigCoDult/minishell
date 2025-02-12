/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:15:26 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/12 13:27:59 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 리다이렉션 토큰 생성
static t_token *create_redir_token(t_shell *shell, const char *value, t_token_type type)
{
    t_token *token;

    debug_print(2047, 3, "DEBUG: Creating redirection token: %s\n", value);
    token = shell_malloc(shell, sizeof(t_token));
    if (!token)
    {
        debug_print(2047, 3, "DEBUG: Failed to allocate redirection token\n");
        return (NULL);
    }
    token->value = shell_strdup(shell, value);
    if (!token->value)
    {
        debug_print(2047, 3, "DEBUG: Failed to duplicate token value\n");
        shell_free(shell, token);
        return (NULL);
    }
    token->type = type;
    token->next = NULL;
    token->prev = NULL;
    return (token);
}

// 리다이렉션 처리 부분
t_token *handle_redirection(t_shell *shell, char **input)
{
    t_token *token;
    t_token *delimiter_token;
    char *word;
    int word_len;

    debug_print(2047, 10, "DEBUG: [handle_redirection] Processing redirection\n");
    
    if (**input == '<' && *(*input + 1) == '<')
    {
        token = create_redir_token(shell, "<<", TOKEN_REDIR);
        *input += 2;
        
        // 공백 건너뛰기
        while (**input && **input == ' ')
            (*input)++;
            
        // delimiter 단어 처리
        word = handle_word(*input, &word_len);
        if (!word)
        {
            debug_print(2047, 9, "DEBUG: Failed to get delimiter word\n");
            shell_free(shell, token);
            return (NULL);
        }
        
        debug_print(2047, 9, "DEBUG: Creating delimiter token: '%s'\n", word);
        delimiter_token = create_token(shell, word, TOKEN_WORD);
        free(word);
        
        if (!delimiter_token)
        {
            debug_print(2047, 9, "DEBUG: Failed to create delimiter token\n");
            shell_free(shell, token);
            return (NULL);
        }
        
        *input += word_len;
        
        // 토큰들을 리스트에 추가
        add_token(shell, token);
        add_token(shell, delimiter_token);
        return (token);
    }
    else if (**input == '>' && *(*input + 1) == '>')
    {
        token = create_redir_token(shell, ">>", TOKEN_REDIR);
        *input += 2;
    }
    else
    {
        char redir[2] = {**input, '\0'};
        token = create_redir_token(shell, redir, TOKEN_REDIR);
        (*input)++;
    }
    if (!token)
    {
        debug_print(2047, 10, "DEBUG: Failed to create redirection token\n");
        return (NULL);
    }
    debug_print(2047, 10, "DEBUG: === REDIRECTION HANDLED ===\n\n");
    return (token);
}