/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:09:14 by sanbaek           #+#    #+#             */
/*   Updated: 2024/03/26 12:09:37 by sanbaek          ###   ########.fr       */
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

static int	ft_is_loweralpha(int c)
{
	return (ft_is_range(c, LOWER_ALPHA_RANGE_START, LOWER_ALPHA_RANGE_END));
}

int	ft_toupper(int c)
{
	if (ft_is_loweralpha(c))
		return (ft_toggle_lettercase(c, -1));
	return (c);
}
