/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:15:26 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/04 05:57:35 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 리다이렉션 토큰 생성
static t_token *create_redir_token(t_shell *shell, const char *value, t_token_type type)
{
    t_token *token;

    printf("DEBUG: Creating redirection token: %s\n", value);
    token = shell_malloc(shell, sizeof(t_token));
    if (!token)
    {
        printf("DEBUG: Failed to allocate redirection token\n");
        return (NULL);
    }
    token->value = shell_strdup(shell, value);
    if (!token->value)
    {
        printf("DEBUG: Failed to duplicate token value\n");
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

    printf("DEBUG: Handling redirection\n");
    if (**input == '<' && *(*input + 1) == '<')
    {
        // heredoc 토큰 생성
        token = create_redir_token(shell, "<<", TOKEN_REDIR);
        if (!token)
            return (NULL);
        
        *input += 2;
        // 공백 건너뛰기
        while (**input && **input == ' ')
            (*input)++;
        
        // 구분자를 별도의 토큰으로 생성
        word = handle_word(*input, &word_len);
        if (!word)
        {
            printf("DEBUG: Failed to get heredoc delimiter\n");
            shell_free(shell, token->value);
            shell_free(shell, token);
            return (NULL);
        }
        
        delimiter_token = create_token(shell, word, TOKEN_WORD);
        free(word);
        if (!delimiter_token)
        {
            shell_free(shell, token->value);
            shell_free(shell, token);
            return (NULL);
        }
        
        // 토큰들을 연결
        token->next = delimiter_token;
        delimiter_token->prev = token;
        
        *input += word_len;
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
    return (token);
}