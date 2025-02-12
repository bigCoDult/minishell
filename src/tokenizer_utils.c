/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 06:04:30 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/12 12:57:24 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 단어의 길이 계산
static int get_word_length(char *input)
{
    int len;

    debug_print(0, 3, "DEBUG: Calculating word length\n");
    len = 0;
    while (input[len] && !strchr(" |<>", input[len]))
    {
        debug_print(0, 3, "DEBUG: Checking character: %c\n", input[len]);
        len++;
    }
    debug_print(0, 3, "DEBUG: Word length: %d\n", len);
    return (len);
}

// 단어 추출
char *handle_word(char *input, int *len)
{
    char *word;

    debug_print(0, 3, "DEBUG: Handling word starting with: %c\n", *input);
    
    // 단어 길이 계산
    *len = get_word_length(input);
    if (*len == 0)
    {
        debug_print(0, 3, "DEBUG: Empty word\n");
        return (NULL);
    }

    // 단어를 위한 메모리 할당
    word = malloc(*len + 1);
    if (!word)
    {
        debug_print(0, 3, "DEBUG: Failed to allocate memory for word\n");
        return (NULL);
    }

    // 단어 복사
    strncpy(word, input, *len);
    word[*len] = '\0';
    
    debug_print(0, 3, "DEBUG: Extracted word: %s\n", word);
    return (word);
}