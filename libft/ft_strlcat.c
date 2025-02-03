/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:28:06 by sanbaek           #+#    #+#             */
/*   Updated: 2024/04/04 19:57:51 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	dest_len;
	size_t	src_len;

	src_len = ft_strlen(src);
	if (dest == NULL && size == 0)
		return (src_len);
	dest_len = ft_strlen(dest);
	if (size == 0 || dest_len >= size)
		return (src_len + size);
	dest += dest_len;
	size -= dest_len;
	while (--size && *src)
		*dest++ = *src++;
	*dest = '\0';
	return (src_len + dest_len);
}
