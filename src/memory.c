/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:24:40 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/04 03:39:40 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 메모리 할당 및 추적 리스트에 추가
void *shell_malloc(t_shell *shell, size_t size)
{
    void *ptr;
    t_memory *new_node;

    // 메모리 할당
    ptr = malloc(size);
    if (!ptr)
        return (NULL);

    // 메모리 추적 노드 생성
    new_node = malloc(sizeof(t_memory));
    if (!new_node)
    {
        free(ptr);
        return (NULL);
    }

    // 노드 초기화 및 리스트 추가
    new_node->addr = ptr;
    new_node->next = shell->memory;
    shell->memory = new_node;

    return (ptr);
}

// 특정 메모리 주소 해제 및 추적 리스트에서 제거
void shell_free(t_shell *shell, void *ptr)
{
    t_memory *current;
    t_memory *prev;

    if (!ptr)
        return;

    prev = NULL;
    current = shell->memory;

    while (current)
    {
        if (current->addr == ptr)
        {
            if (prev)
                prev->next = current->next;
            else
                shell->memory = current->next;
            free(ptr);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// 모든 할당된 메모리 해제
void free_all_memory(t_shell *shell)
{
    t_memory *current;
    t_memory *next;

    current = shell->memory;
    while (current)
    {
        next = current->next;
        free(current->addr);
        free(current);
        current = next;
    }
    shell->memory = NULL;
}