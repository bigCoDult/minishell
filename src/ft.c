/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 03:48:14 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/04 03:55:44 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strchr(const char *str, int c)
{
    while (*str)
    {
        if (*str == (char)c)
            return ((char *)str);
        str++;
    }
    if (*str == (char)c)
        return ((char *)str);
    return (NULL);
}

int ft_strlen(const char *str)
{
    int len;

    len = 0;
    while (str[len])
        len ++;
    return (len);
}

// strdup 대신 사용할 함수
char *shell_strdup(t_shell *shell, const char *str)
{
    char *dup;
    size_t len;

    len = ft_strlen(str) + 1;
    dup = shell_malloc(shell, len);
    if (!dup)
        return (NULL);
    memcpy(dup, str, len);
    return (dup);
}