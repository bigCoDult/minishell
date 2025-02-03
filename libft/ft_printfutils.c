/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pututils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 10:28:54 by sanbaek           #+#    #+#             */
/*   Updated: 2024/08/18 10:28:54 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printfchar_fd(char col, int fd)
{
	write(fd, &col, 1);
	return (1);
}

int	ft_printfstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
	return (ft_strlen(s));
}

int	ft_printfll_fd(long long int n, int fd)
{
	int	count;

	count = 1;
	if (n < 0)
	{
		n *= -1;
		write(fd, "-", 1);
		count++;
	}
	if (n > 9)
		count += ft_printfll_fd(n / 10, fd);
	n = n % 10 + '0';
	write(fd, &n, 1);
	return (count);
}

int	ft_printfhex_fd(int is_x, long long unsigned int n, int fd)
{
	char	digit;
	int		count;

	digit = '\0';
	count = 1;
	if (n >= 16)
		count += ft_printfhex_fd(is_x, n / 16, fd);
	n = n % 16;
	if (n < 10)
		digit = n + '0';
	else if (is_x == 'x')
		digit = n + 'a' - 10;
	else if (is_x == 'X')
		digit = n + 'A' - 10;
	write(fd, &digit, 1);
	return (count);
}
