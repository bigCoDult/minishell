/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 20:13:40 by sanbaek           #+#    #+#             */
/*   Updated: 2024/03/25 21:43:23 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_range(int c, int start, int end)
{
	return (c >= start && c <= end);
}

static int	ft_is_loweralpha(int c)
{
	return (ft_is_range(c, LOWER_ALPHA_RANGE_START, LOWER_ALPHA_RANGE_END));
}

static int	ft_is_upperalpha(int c)
{
	return (ft_is_range(c, UPPER_ALPHA_RANGE_START, UPPER_ALPHA_RANGE_END));
}

int	ft_isalpha(int c)
{
	return (ft_is_loweralpha(c) || ft_is_upperalpha(c));
}
