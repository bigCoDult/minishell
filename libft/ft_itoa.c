/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 09:48:50 by sanbaek           #+#    #+#             */
/*   Updated: 2024/04/02 09:48:50 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_get_bigmin_str(void);
static void	ft_set_min_info(t_min_info *min_info, int n);

char	*ft_itoa(int n)
{
	t_min_info	min_info;
	char		*str;

	if (n == -2147483648)
		return (ft_get_bigmin_str());
	min_info.minus_flag = 0;
	min_info.length = 0;
	min_info.n1 = 0;
	min_info.n2 = 0;
	ft_set_min_info(&min_info, n);
	str = (char *)ft_calloc(min_info.length + 1, sizeof(char));
	if (str == NULL)
		return (NULL);
	if (min_info.minus_flag == 1)
		str[0] = '-';
	while (min_info.length-- > min_info.minus_flag)
	{
		str[min_info.length] = (char)((min_info.n2 % 10) + '0');
		min_info.n2 /= 10;
	}
	return (str);
}

static char	*ft_get_bigmin_str(void)
{
	char	*str;

	str = (char *)ft_calloc(11 + 1, sizeof(char));
	if (str == NULL)
		return (NULL);
	ft_memmove(str, "-2147483648", 11);
	return (str);
}

static void	ft_set_min_info(t_min_info *min_info, int n)
{
	if (n < 0)
	{
		min_info->minus_flag = 1;
		min_info->length = 2;
		min_info->n1 = -n;
		min_info->n2 = -n;
	}
	else
	{
		min_info->minus_flag = 0;
		min_info->length = 1;
		min_info->n1 = n;
		min_info->n2 = n;
	}
	while (min_info->n1 > 9)
	{
		min_info->n1 /= 10;
		min_info->length++;
	}
	return ;
}
