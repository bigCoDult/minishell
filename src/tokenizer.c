/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:07:22 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/08 10:41:57 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 토큰 생성
t_token *create_token(t_shell *shell, char *value, t_token_type type)
{
    t_token *token;

    printf("DEBUG: Creating token with value: %s, type: %d\n", value, type);
    token = shell_malloc(shell, sizeof(t_token));
    if (!token)
    {
        printf("DEBUG: Failed to allocate token\n");
        return (NULL);
    }
    token->value = shell_strdup(shell, value);
    if (!token->value)
    {
        printf("DEBUG: Failed to duplicate token value\n");
        return (NULL);
    }
    token->type = type;
    token->next = NULL;
    token->prev = NULL;
    printf("DEBUG: Token created successfully\n");
    return (token);
}

// 토큰 리스트에 새 토큰 추가
void add_token(t_shell *shell, t_token *token)
{
    t_token *current;

    printf("\n=== ADD TOKEN ===\n");
    printf("Adding token: type=%d, value='%s'\n", token->type, token->value);
    
    if (!shell->tokens)
    {
        printf("First token in list\n");
        shell->tokens = token;
    }
    else
    {
        current = shell->tokens;
        while (current->next)
            current = current->next;
        printf("Adding after token: type=%d, value='%s'\n", 
               current->type, current->value);
        current->next = token;
        token->prev = current;
    }
    
    // 현재 토큰 리스트 출력
    printf("Current token list:\n");
    current = shell->tokens;
    while (current)
    {
        printf("- type=%d, value='%s'\n", current->type, current->value);
        current = current->next;
    }
    printf("=== TOKEN ADDED ===\n\n");
}

int tokenize_input(t_shell *shell)
{
    char *input;
    t_token *token;
    char *word;
    int word_len;

    printf("DEBUG: [tokenize_input] Starting tokenization\n");
    input = shell->input_line;
    
    while (*input)
    {
        while (*input && *input == ' ')
            input++;
        if (!*input)
            break;

        if (*input == '|')
        {
            token = create_token(shell, "|", TOKEN_PIPE);
            add_token(shell, token);
            input++;
        }
        else if (*input == '<' || *input == '>')
        {
            token = handle_redirection(shell, &input);
            if (!token)
                return (1);
            // handle_redirection 내부에서 토큰 추가 처리
        }
        else
        {
            word = handle_word(input, &word_len);
            if (!word)
                return (1);
            token = create_token(shell, word, TOKEN_WORD);
            free(word);
            input += word_len;
            if (!token)
                return (1);
            add_token(shell, token);
        }
    }
    return (0);
}
