/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 10:28:19 by sanbaek           #+#    #+#             */
/*   Updated: 2024/08/18 10:28:19 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf(const char *str, ...)
{
	va_list	ap;
	t_etc	*etc;
	int		final_count;

	etc = malloc(sizeof(t_etc));
	if (etc == NULL)
		return (-1);
	va_start(ap, str);
	etc->n = 0;
	etc->u = 0;
	etc->string = "";
	etc->ads = NULL;
	while (*str)
	{
		if (*str == '%')
			ft_printffmt_cdux(etc, ap, *++str);
		else
			etc->n += ft_printfchar_fd(*str, 1);
		str++;
		va_end(ap);
	}
	final_count = etc->n;
	free(etc);
	return (final_count);
}

int	ft_printffmt_cdux(t_etc *etc, va_list ap, char c)
{
	if (c == '%')
		etc->n += ft_printfchar_fd('%', 1);
	else if (c == 'c')
		etc->n += ft_printfchar_fd(va_arg(ap, int), 1);
	else if (c == 'd' || c == 'i')
		etc->n += ft_printfll_fd(va_arg(ap, int), 1);
	else if (c == 'u')
		etc->n += ft_printfll_fd(va_arg(ap, unsigned int), 1);
	else if (c == 'x' || c == 'X')
		etc->n += ft_printfhex_fd(c, va_arg(ap, unsigned int), 1);
	else
		return (ft_printffmt_sp(etc, ap, c));
	return (etc->n);
}

int	ft_printffmt_sp(t_etc *etc, va_list ap, char c)
{
	if (c == 's')
	{
		etc->string = (va_arg(ap, char *));
		if (etc->string == NULL)
			etc->n += ft_printfstr_fd("(null)", 1);
		else
			etc->n += ft_printfstr_fd(etc->string, 1);
	}
	else if (c == 'p')
	{
		etc->ads = va_arg(ap, void *);
		if (etc->ads == NULL)
			etc->n += ft_printfstr_fd("(nil)", 1);
		else
		{
			etc->n += ft_printfstr_fd("0x", 1);
			etc->n += ft_printfhex_fd('x', (long long unsigned int)etc->ads, 1);
		}
	}
	return (etc->n);
}
