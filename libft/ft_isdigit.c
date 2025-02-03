/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 20:11:02 by sanbaek           #+#    #+#             */
/*   Updated: 2024/03/25 20:11:04 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_range(int c, int start, int end)
{
	return (c >= start && c <= end);
}

int	ft_isdigit(int c)
{
	return (ft_is_range(c, DIGIT_RANGE_START, DIGIT_RANGE_END));
}
