/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 03:48:14 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/10 02:16:14 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strchr(const char *str, int c)
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

int	ft_strlen(const char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len ++;
	return (len);
}

char	*shell_strdup(t_shell *shell, const char *str)
{
	char	*dup;
	size_t	len;

	len = ft_strlen(str) + 1;
	dup = shell_malloc(shell, len);
	if (!dup)
		return (NULL);
	memcpy(dup, str, len);
	return (dup);
}

char	*ft_strdup(const char *str)
{
	char	*dup;
	size_t	len;

	len = ft_strlen(str) + 1;
	dup = malloc(len);
	if (!dup)
		return (NULL);
	memcpy(dup, str, len);
	return (dup);
}

void	*ft_memset(void *dest, int c, size_t count)
{
	unsigned char	*dest_tmp;
	unsigned char	unsigned_char_c;

	dest_tmp = (unsigned char *)dest;
	unsigned_char_c = (unsigned char)c;
	while (count--)
		*dest_tmp++ = unsigned_char_c;
	return (dest);
}

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}
