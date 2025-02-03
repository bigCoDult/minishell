/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 18:45:40 by sanbaek           #+#    #+#             */
/*   Updated: 2024/03/27 18:45:40 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	size_t	length;
	char	*dup_src;
	int		i;

	length = ft_strlen(src);
	dup_src = (char *)ft_calloc(length + 1, sizeof(char));
	if (dup_src == NULL)
		return (NULL);
	i = 0;
	while (src[i] != '\0')
	{
		dup_src[i] = (char)src[i];
		i++;
	}
	return (dup_src);
}
