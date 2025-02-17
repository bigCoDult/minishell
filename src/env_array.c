/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 05:07:15 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/17 20:22:11 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// // 환경 변수 개수 세기
// static int count_env_vars(t_env *env)
// {
//     int count;
//     t_env *current;

//     count = 0;
//     current = env;
//     while (current)
//     {
//         count++;
//         current = current->next;
//     }
//     return (count);
// }

// // 환경 변수 문자열 생성 (KEY=VALUE 형식)
// static char *create_env_string(t_shell *shell, const char *key, const char *value)
// {
//     char *env_str;
//     size_t len;

//     // key + '=' + value + '\0' 길이 계산
//     len = strlen(key) + strlen(value) + 2;
    
//     env_str = shell_malloc(shell, len);
//     if (!env_str)
//         return (NULL);

//     // KEY=VALUE 형식으로 문자열 생성
//     sprintf(env_str, "%s=%s", key, value);
//     return (env_str);
// }

// // 환경 변수 리스트를 문자열 배열로 변환
// char **get_env_array(t_shell *shell)
// {
//     char **env_array;
//     t_env *current;
//     int count;
//     int i;

//     printf("DEBUG: get_env_array start\n");

//     count = count_env_vars(shell->env);
//     printf("DEBUG: Environment variable count: %d\n", count);

//     env_array = shell_malloc(shell, sizeof(char *) * (count + 1));
//     if (!env_array)
//     {
//         printf("DEBUG: Failed to allocate env_array\n");
//         return (NULL);
//     }

//     current = shell->env;
//     i = 0;
//     while (current)
//     {
//         printf("DEBUG: Creating env string for: %s\n", current->key);
//         env_array[i] = create_env_string(shell, current->key, current->value);
//         if (!env_array[i])
//         {
//             printf("DEBUG: Failed to create env string\n");
//             return (NULL);
//         }
//         current = current->next;
//         i++;
//     }
//     env_array[i] = NULL;

//     printf("DEBUG: get_env_array completed successfully\n");
//     return (env_array);
// }

// 환경 변수 배열 생성
char **get_env_array(t_shell *shell)
{
    char **env_array;
    t_env *current;
    int count;

    debug_print(0, 2, "DEBUG: Creating environment array\n");
    
    // 환경 변수 개수 세기
    count = 0;
    current = shell->env;
    while (current)
    {
        count++;
        current = current->next;
    }

    // 배열 할당
    env_array = shell_malloc(shell, sizeof(char *) * (count + 1));
    if (!env_array)
        return (NULL);

    // 환경 변수 문자열 생성
    current = shell->env;
    count = 0;
    while (current)
    {
        env_array[count] = shell_malloc(shell, 
            strlen(current->key) + strlen(current->value) + 2);
        if (!env_array[count])
            return (NULL);
        
        sprintf(env_array[count], "%s=%s", current->key, current->value);
        count++;
        current = current->next;
    }
    env_array[count] = NULL;

    return (env_array);
}