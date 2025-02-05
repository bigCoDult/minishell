/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 03:33:17 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/05 13:25:06 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 환경 변수 값 설정
void set_env_value(t_shell *shell, const char *key, const char *value)
{
    t_env *current;
    t_env *new_node;

    // 기존 환경 변수 찾기
    current = shell->env;
    while (current)
    {
        if (strcmp(current->key, key) == 0)
        {
            // 키가 존재하면 값만 업데이트
            shell_free(shell, current->value);
            current->value = shell_strdup(shell, value);
            return;
        }
        current = current->next;
    }

    // 키가 없으면 새 노드 생성
    new_node = shell_malloc(shell, sizeof(t_env));
    if (!new_node)
        return;

    // 새 노드 초기화
    new_node->key = shell_strdup(shell, key);
    new_node->value = shell_strdup(shell, value);
    new_node->next = NULL;

    // 리스트에 새 노드 추가
    if (!shell->env)
    {
        shell->env = new_node;
    }
    else
    {
        current = shell->env;
        while (current->next)
            current = current->next;
        current->next = new_node;
    }
}

static t_env *create_env_node(t_shell *shell, char *key, char *value)
{
    t_env *new_node;


    (void)shell;
    new_node = malloc(sizeof(t_env));
    if (!new_node)
        return (NULL);
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = NULL;
    return (new_node);
}

static void add_env_node(t_env **env_list, t_env *new_node)
{
    t_env *current;

    if (!*env_list)
    {
        *env_list = new_node;
        return;
    }
    current = *env_list;
    while (current->next)
        current = current->next;
    current->next = new_node;
}

t_env *init_env(t_shell *shell, char **envp)
{
    t_env *env_list;
    char *equal_sign;
    char *key;
    char *value;
    int i;

    env_list = NULL;
    i = 0;
    while (envp[i])
    {
        equal_sign = ft_strchr(envp[i], '=');
        if (equal_sign)
        {
            *equal_sign = '\0';  // 임시로 문자열 분리
            key = envp[i];
            value = equal_sign + 1;
            add_env_node(&env_list, create_env_node(shell, key, value));
            *equal_sign = '=';   // 원래 문자열 복구
        }
        i++;
    }
    return (env_list);
}