/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:08:03 by sanbaek           #+#    #+#             */
/*   Updated: 2024/03/26 12:08:13 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_range(int c, int start, int end)
{
	return (c >= start && c <= end);
}

static int	ft_toggle_lettercase(int c, int case_to)
{
	return (c + LETTER_CASE_INTERVAL * case_to);
}

static int	ft_is_upperalpha(int c)
{
	return (ft_is_range(c, UPPER_ALPHA_RANGE_START, UPPER_ALPHA_RANGE_END));
}

int	ft_tolower(int c)
{
	if (ft_is_upperalpha(c))
		return (ft_toggle_lettercase(c, +1));
	return (c);
}
