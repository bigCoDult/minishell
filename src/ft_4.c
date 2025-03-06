/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 18:57:35 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 19:04:59 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_int_min(char *str, size_t size)
{
	if (size >= 12)
	{
		ft_strcpy(str, "-2147483648");
		return (11);
	}
	else if (size > 0)
	{
		ft_strncpy(str, "-2147483648", size - 1);
		str[size - 1] = '\0';
		return (11);
	}
	return (0);
}

int	calculate_num_length(int n, int *is_negative)
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

void	fill_number_buffer(char *buffer, int n, int len, int is_negative)
{
	int	i;

	i = len - 1 + is_negative;
	buffer[i + 1] = '\0';
	while (n > 0)
	{
		buffer[i] = '0' + (n % 10);
		n = n / 10;
		i--;
	}
	if (is_negative)
		buffer[0] = '-';
}

int	copy_to_output(char *dest, const char *src, size_t size, int total_len)
{
	int	i;
	int	copy_len;

	if (total_len < (int)size - 1)
		copy_len = total_len;
	else
		copy_len = (int)size - 1;
	i = 0;
	while (i < copy_len)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (total_len);
}

int	ft_itoa_n(char *str, size_t size, int n)
{
	int		result;
	int		is_negative;
	int		len;
	int		value;
	char	buffer[12];

	result = handle_special_cases(str, size, n);
	if (result >= 0)
		return (result);
	if (n == -2147483648)
		return (handle_int_min(str, size));
	len = calculate_num_length(n, &is_negative);
	if (n < 0)
		value = -n;
	else
		value = n;
	fill_number_buffer(buffer, value, len, is_negative);
	return (copy_to_output(str, buffer, size, len + is_negative));
}
