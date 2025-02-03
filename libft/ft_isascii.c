/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 21:39:00 by sanbaek           #+#    #+#             */
/*   Updated: 2024/03/25 21:39:13 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_range(int c, int start, int end)
{
	return (c >= start && c <= end);
}

int	ft_isascii(int c)
{
	return (ft_is_range(c, ASCII_RANGE_START, ASCII_RANGE_END));
}
