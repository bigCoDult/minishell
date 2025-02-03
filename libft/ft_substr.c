/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 21:33:50 by sanbaek           #+#    #+#             */
/*   Updated: 2024/03/27 21:33:50 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_s;
	size_t	i;
	size_t	s_len;

	s_len = ft_strlen(s);
	if (start >= s_len)
	{
		sub_s = (char *)ft_calloc(1, sizeof(char));
		return (sub_s);
	}
	if (len > s_len - start)
		len = s_len - start;
	sub_s = (char *)ft_calloc(len + 1, sizeof(char));
	if (sub_s == NULL)
		return (NULL);
	i = 0;
	while (i < len && s[start + i] != '\0')
	{
		sub_s[i] = s[start + i];
		i++;
	}
	return (sub_s);
}
