/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:31:52 by sanbaek           #+#    #+#             */
/*   Updated: 2024/04/04 19:58:28 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp( const char *lhs, const char *rhs, size_t count )
{
	size_t	i;

	i = 0;
	if (count == 0)
		return (0);
	while (--count && lhs[i] == rhs[i] && (lhs[i] && rhs[i]))
		i++;
	return (((unsigned char)lhs[i] - (unsigned char)rhs[i]));
}
