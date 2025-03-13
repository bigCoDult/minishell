/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_7.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 03:06:22 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 03:06:45 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_memcpy(void *dest, const void *src, size_t count)
{
	unsigned char		*dest_tmp;
	const unsigned char	*src_tmp;

	dest_tmp = (unsigned char *)dest;
	src_tmp = (const unsigned char *)src;
	while (count--)
		*dest_tmp++ = *src_tmp++;
	return (dest);
}

char	*ft_strndup(const char *s, size_t n, t_shell *shell)
{
	char	*dup;
	size_t	len;
	size_t	i;

	len = 0;
	while (len < n && s[len])
		len++;
	dup = (char *)shell_malloc(shell, len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	k;

	if (*little == '\0')
		return ((char *)big);
	i = 0;
	while (i < len && big[i])
	{
		k = 0;
		while (big[i + k] == little[k] && i + k < len)
		{
			k++;
			if (little[k] == '\0')
				return ((char *)(big + i));
		}
		i++;
	}
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2, t_shell *shell)
{
	char	*join_s;
	size_t	i;

	join_s = (char *)shell_malloc(shell, \
		(ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	i = 0;
	while (*s1)
	{
		join_s[i] = *s1;
		i++;
		s1++;
	}
	while (*s2)
	{
		join_s[i] = *s2;
		i++;
		s2++;
	}
	join_s[i] = '\0';
	return (join_s);
}
