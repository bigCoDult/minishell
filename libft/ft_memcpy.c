/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:23:16 by sanbaek           #+#    #+#             */
/*   Updated: 2024/03/26 12:23:18 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
