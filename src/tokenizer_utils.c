/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 06:04:30 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/04 06:04:32 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 단어의 길이 계산
static int get_word_length(char *input)
{
    int len;

    printf("DEBUG: Calculating word length\n");
    len = 0;
    while (input[len] && !strchr(" |<>", input[len]))
    {
        printf("DEBUG: Checking character: %c\n", input[len]);
        len++;
    }
    printf("DEBUG: Word length: %d\n", len);
    return (len);
}

// 단어 추출
char *handle_word(char *input, int *len)
{
    char *word;

    printf("DEBUG: Handling word starting with: %c\n", *input);
    
    // 단어 길이 계산
    *len = get_word_length(input);
    if (*len == 0)
    {
        printf("DEBUG: Empty word\n");
        return (NULL);
    }

    // 단어를 위한 메모리 할당
    word = malloc(*len + 1);
    if (!word)
    {
        printf("DEBUG: Failed to allocate memory for word\n");
        return (NULL);
    }

    // 단어 복사
    strncpy(word, input, *len);
    word[*len] = '\0';
    
    printf("DEBUG: Extracted word: %s\n", word);
    return (word);
}