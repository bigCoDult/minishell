/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_6.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 03:05:32 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/07 06:40:44 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_delimiter(char c, const char *delim)
{
	while (*delim != '\0')
	{
		if (c == *delim)
			return (1);
		delim++;
	}
	return (0);
}

char	*skip_delimiters(char *str, const char *delim)
{
	char	*ptr;

	ptr = str;
	while (*ptr != '\0')
	{
		if (!is_delimiter(*ptr, delim))
			break ;
		ptr++;
	}
	return (ptr);
}

char	*find_next_delimiter(char *str, const char *delim)
{
	while (*str != '\0')
	{
		if (is_delimiter(*str, delim))
			break ;
		str++;
	}
	return (str);
}

char	*terminate_token(char *delimiter_pos)
{
	if (*delimiter_pos != '\0')
	{
		*delimiter_pos = '\0';
		return (delimiter_pos + 1);
	}
	return (delimiter_pos);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*last_token = NULL;
	char		*token_start;

	if (str != NULL)
		last_token = str;
	if (last_token == NULL || *last_token == '\0')
		return (NULL);
	last_token = skip_delimiters(last_token, delim);
	if (*last_token == '\0')
		return (NULL);
	token_start = last_token;
	last_token = find_next_delimiter(last_token, delim);
	if (*last_token != '\0')
	{
		*last_token = '\0';
		last_token++;
	}
	return (token_start);
}
