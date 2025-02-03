/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:26:26 by sanbaek           #+#    #+#             */
/*   Updated: 2024/04/04 19:57:57 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *lhs, const void *rhs, size_t size)
{
	unsigned char	*lhs_tmp;
	unsigned char	*rhs_tmp;
	size_t			i;

	rhs_tmp = (unsigned char *)rhs;
	lhs_tmp = (unsigned char *)lhs;
	i = 0;
	if (size == 0)
		return (0);
	while (--size && lhs_tmp[i] == rhs_tmp[i])
		i++;
	return ((lhs_tmp[i] - rhs_tmp[i]));
}
