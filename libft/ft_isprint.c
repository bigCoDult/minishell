/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 21:38:25 by sanbaek           #+#    #+#             */
/*   Updated: 2024/03/25 21:38:36 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_range(int c, int start, int end)
{
	return (c >= start && c <= end);
}

int	ft_isprint(int c)
{
	return (ft_is_range(c, PRINTABLE_RANGE_START, PRINTABLE_RANGE_END));
}
