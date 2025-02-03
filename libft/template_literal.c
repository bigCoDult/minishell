/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   template_literal.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:22:28 by sanbaek           #+#    #+#             */
/*   Updated: 2024/12/30 13:22:28 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*template_literal(char *line, char *word, int location)
{
	char	*str;
	int		str_i;
	int		line_i;
	int		word_i;

	str = malloc(sizeof(char) * (ft_strlen(line) + ft_strlen(word) + 1));
	if (str == NULL)
		return (NULL);
	str_i = 0;
	line_i = 0;
	word_i = 0;
	while (line_i < location)
		str[str_i++] = line[line_i++];
	while (word_i < (int)ft_strlen(word))
		str[str_i++] = word[word_i++];
	while (line[line_i] != '\0')
		str[str_i++] = line[line_i++];
	str[str_i] = '\0';
	return (str);
}
