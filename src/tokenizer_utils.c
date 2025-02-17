/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 06:04:30 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/17 20:38:57 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 단어의 길이 계산
static int get_word_length(char *input)
{
    int len;

    debug_print(2047, 3, "DEBUG: Calculating word length\n");
    len = 0;
    while (input[len] && !strchr(" |<>", input[len]))
    {
        // 따옴표 처리 추가
        if (input[len] == '"' || input[len] == '\'')
        {
            char quote = input[len];
            len++;
            while (input[len] && input[len] != quote)
                len++;
            if (input[len])
                len++;
            continue;
        }
        debug_print(2047, 3, "DEBUG: Checking character: %c\n", input[len]);
        len++;
    }
    debug_print(2047, 3, "DEBUG: Word length: %d\n", len);
    return (len);
}

// 단어 추출
char *handle_word(t_shell *shell, char *input, int *len)
{
    char *word;
    
    debug_print(2047, 3, "DEBUG: Handling word starting with: %c\n", *input);
    
    *len = get_word_length(input);
    if (*len == 0)
    {
        debug_print(2047, 3, "DEBUG: Empty word\n");
        return NULL;
    }

    word = shell_malloc(shell, *len + 1);
    if (!word)
    {
        debug_print(2047, 3, "DEBUG: Failed to allocate memory for word\n");
        return NULL;
    }

    strncpy(word, input, *len);
    word[*len] = '\0';

    // 환경변수 확장
    char *expanded = expand_env_var(shell, word);
    shell_free(shell, word);
    
    debug_print(2047, 3, "DEBUG: Expanded word: %s\n", expanded);
    return expanded;
}

// // 따옴표 처리 함수
// static char *handle_quoted_string(t_shell *shell, char *input, int *len, char quote)
// {
//     char *word;
//     int i = 1;  // 시작 따옴표 건너뛰기

//     // 닫는 따옴표 찾기
//     while (input[i] && input[i] != quote)
//         i++;
    
//     if (!input[i])  // 닫는 따옴표가 없는 경우
//         return NULL;

//     // 따옴표 내용 추출
//     word = shell_malloc(shell, i);
//     strncpy(word, input + 1, i - 1);
//     word[i - 1] = '\0';

//     // 큰따옴표 내부에서만 환경변수 확장
//     if (quote == '"')
//     {
//         char *expanded = expand_env_var(shell, word);
//         shell_free(shell, word);
//         word = expanded;
//     }

//     *len = i + 1;  // 전체 길이 (따옴표 포함)
//     return word;
// }