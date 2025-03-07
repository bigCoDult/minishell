/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_5.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 02:45:46 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/07 02:45:50 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_zero(char *str, int n)
{
	if (n == 0)
	{
		str[0] = '0';
		str[1] = '\0';
	}
}

int	calculate_number_length(int n, int *is_negative)
{
	int	len;
	int	temp;

	*is_negative = 0;
	if (n < 0)
	{
		*is_negative = 1;
		n = -n;
	}
	len = 0;
	temp = n;
	while (temp > 0)
	{
		temp = temp / 10;
		len++;
	}
	return (len);
}

void	convert_number_to_string(char *str, int n, int len, int is_negative)
{
	int	i;

	if (is_negative)
		str[0] = '-';
	i = len - 1 + is_negative;
	str[i + 1] = '\0';
	while (n > 0)
	{
		str[i] = '0' + (n % 10);
		n = n / 10;
		i--;
	}
}

void	ft_itoa_simple(char *str, int n)
{
	int	len;
	int	is_negative;

	if (n == 0)
	{
		handle_zero(str, n);
		return ;
	}
	len = calculate_number_length(n, &is_negative);
	if (n < 0 && n != -2147483648)
		n = -n;
	else if (n == -2147483648)
	{
		ft_strcpy(str, "-2147483648");
		return ;
	}
	convert_number_to_string(str, n, len, is_negative);
}
