/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaei <sanbaei@student.42gyeongsan.ir    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 22:38:53 by sanbaei           #+#    #+#             */
/*   Updated: 2024/03/27 22:38:53 by sanbaei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trim_s1;
	size_t	start;
	size_t	end;
	size_t	i;

	i = 0;
	start = 0;
	end = ft_strlen(s1);
	while (set[i] != '\0')
	{
		while (start <= end && ft_strchr(set, s1[start]) != NULL)
			start++;
		while (start <= end && ft_strrchr(set, s1[end]) != NULL)
			end--;
		i++;
	}
	trim_s1 = (char *)ft_calloc((end - start + 1) + 1, sizeof(char));
	if (trim_s1 == NULL)
		return (NULL);
	ft_memmove(trim_s1, s1 + start, end - start + 1);
	return (trim_s1);
}
