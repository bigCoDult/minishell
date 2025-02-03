/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 09:33:22 by sanbaek           #+#    #+#             */
/*   Updated: 2024/04/02 09:33:22 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Pass characters and indexes from the string to the function
// Run a function for each index

// Applies the function ’f’ on each character of the string passed as argument,
// passing its index as first argument.
// Each character is passed by address to ’f’ to be modified if necessary.
void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	while (s[i])
	{
		f(i, s + i);
		i++;
	}
	return ;
}
