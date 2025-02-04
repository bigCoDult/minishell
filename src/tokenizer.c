/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:07:22 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/04 06:05:32 by yutsong          ###   ########.fr       */
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
static void add_token(t_shell *shell, t_token *new_token)
{
    t_token *current;

    printf("DEBUG: Adding token to list\n");
    if (!shell->tokens)
    {
        shell->tokens = new_token;
        printf("DEBUG: Added as first token\n");
        return;
    }
    current = shell->tokens;
    while (current->next)
        current = current->next;
    current->next = new_token;
    new_token->prev = current;
    printf("DEBUG: Token added to list\n");
}

int tokenize_input(t_shell *shell)
{
    char *input;
    t_token *token;
    int word_len;
    char *word;
    
    printf("DEBUG: Starting tokenization\n");
    input = shell->input_line;
    if (!input)
    {
        printf("DEBUG: No input line\n");
        return (1);
    }

    printf("DEBUG: Processing input: %s\n", input);
    while (*input)
    {
        // 공백 건너뛰기
        while (*input && *input == ' ')
            input++;
        if (!*input)
            break;

        printf("DEBUG: Processing character: %c\n", *input);
        // 파이프 체크
        if (*input == '|')
        {
            printf("DEBUG: Found pipe token\n");
            token = create_token(shell, "|", TOKEN_PIPE);
            input++;
        }
        // 리다이렉션 체크
        else if (*input == '<' || *input == '>')
        {
            printf("DEBUG: Found redirection\n");
            token = handle_redirection(shell, &input);
            if (!token)
                return (1);
            add_token(shell, token);
        }
        // 일반 단어
        else
        {
            printf("DEBUG: Processing word token\n");
            word = handle_word(input, &word_len);
            if (!word)
            {
                printf("DEBUG: Failed to handle word\n");
                return (1);
            }
            token = create_token(shell, word, TOKEN_WORD);
            free(word);
            input += word_len;
        }
        
        if (!token)
        {
            printf("DEBUG: Failed to create token\n");
            return (1);
        }
        add_token(shell, token);
    }

    printf("DEBUG: Tokenization completed successfully\n");
    return (0);
}
