/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 18:13:57 by sanbaek           #+#    #+#             */
/*   Updated: 2024/03/27 18:13:57 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../parse/memory.h"

void	*ft_calloc(size_t num, size_t size)
{
	void	*ptr;

	if (num != 0 && (num * size) / num != size)
		return (NULL);
	ptr = ft_malloc(num * size);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, num * size);
	return (ptr);
}
