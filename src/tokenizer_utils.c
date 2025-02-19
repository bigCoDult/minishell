/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 06:04:30 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/19 04:07:47 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 토큰 처리 상태를 추적하기 위한 구조체
typedef struct s_token_state {
    int in_single_quote;  // 작은따옴표 안인지
    int in_double_quote;  // 큰따옴표 안인지
} t_token_state;

// 단어의 길이 계산
static int get_word_length(char *input)
{
    int len;
    t_token_state state;

    debug_print(2047, 3, "DEBUG: Calculating word length\n");
    len = 0;
    state.in_single_quote = 0;
    state.in_double_quote = 0;

    while (input[len])
    {
        // 따옴표 처리
        if (input[len] == '"' && !state.in_single_quote)
        {
            state.in_double_quote = !state.in_double_quote;
            len++;
            continue;
        }
        if (input[len] == '\'' && !state.in_double_quote)
        {
            state.in_single_quote = !state.in_single_quote;
            len++;
            continue;
        }

        // 단어 끝 확인 (따옴표 안이 아닐 때만)
        if (!state.in_single_quote && !state.in_double_quote && 
            strchr(" |<>", input[len]))
            break;

        debug_print(2047, 3, "DEBUG: Checking character: %c\n", input[len]);
        len++;
    }

    // 닫히지 않은 따옴표 체크
    if (state.in_single_quote || state.in_double_quote)
    {
        debug_print(2047, 3, "DEBUG: Unclosed quote detected\n");
        // 나중에 에러 처리 추가
    }

    debug_print(2047, 3, "DEBUG: Word length: %d\n", len);
    return (len);
}

// 단어 추출
char *handle_word(t_shell *shell, char *input, int *len)
{
    char *word;
    char *result;
    t_token_state state;
    int i, j;
    
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

    // 따옴표를 제거하면서 단어 복사
    state.in_single_quote = 0;
    state.in_double_quote = 0;
    i = 0;
    j = 0;

    while (i < *len)
    {
        if (input[i] == '"' && !state.in_single_quote)
        {
            state.in_double_quote = !state.in_double_quote;
            i++;
            continue;
        }
        if (input[i] == '\'' && !state.in_double_quote)
        {
            state.in_single_quote = !state.in_single_quote;
            i++;
            continue;
        }
        word[j++] = input[i++];
    }
    word[j] = '\0';

    // 환경변수 확장 (작은따옴표 안이 아닐 때만)
    if (!state.in_single_quote)
        result = expand_env_var(shell, word);
    else
        result = shell_strdup(shell, word);

    shell_free(shell, word);
    debug_print(2047, 3, "DEBUG: Expanded word: %s\n", result);
    return result;
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