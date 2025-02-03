/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:24:14 by sanbaek           #+#    #+#             */
/*   Updated: 2024/03/26 12:24:19 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t count)
{
	unsigned char		*dest_tmp;
	const unsigned char	*src_tmp;

	dest_tmp = (unsigned char *)dest;
	src_tmp = (const unsigned char *)src;
	if (dest_tmp > src_tmp)
	{
		while (count--)
			*(dest_tmp + count) = *(src_tmp + count);
	}
	else
	{
		while (count--)
			*dest_tmp++ = *src_tmp++;
	}
	return (dest);
}
