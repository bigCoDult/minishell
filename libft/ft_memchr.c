/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:34:58 by sanbaek           #+#    #+#             */
/*   Updated: 2024/03/26 12:35:01 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *ptr, int c, size_t count)
{
	unsigned char	*char_ptr;
	unsigned char	char_c;

	char_ptr = (unsigned char *)ptr;
	char_c = (unsigned char)c;
	while (count--)
	{
		if (*char_ptr == char_c)
			return ((void *)(char_ptr));
		char_ptr++;
	}
	return (NULL);
}
