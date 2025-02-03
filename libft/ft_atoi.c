/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 22:39:31 by sanbaek           #+#    #+#             */
/*   Updated: 2024/03/26 22:39:36 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isspace(char c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	ft_atoi(char *str)
{
	long long	rest;
	int			sign;
	int			length;

	rest = 0;
	sign = 1;
	length = 0;
	while (str[length] && ft_isspace(str[length]))
		length++;
	if (str[length] == '-')
	{
		sign = sign * -1;
		length++;
	}
	else if (str[length] == '+')
		length++;
	while (str[length] && (str[length] >= '0' && str[length] <= '9'))
	{
		rest = rest * 10 + str[length] - '0';
		length++;
	}
	return (sign * (int)(rest));
}
