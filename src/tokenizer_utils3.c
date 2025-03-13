/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 07:52:12 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 08:08:03 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_character(t_shell *shell,
	char *word, char *temp, int params[5])
{
	int	indices[2];

	if (params[4] == QUOTE_CHAR)
	{
		if ((word[params[0]] == '\'' && !params[3])
			|| (word[params[0]] == '"' && !params[2]))
			update_quote_state(word, &params[0], &params[2], &params[3]);
		else
			temp[(params[1])++] = word[(params[0])++];
	}
	else if (params[4] == ENV_VAR)
	{
		indices[0] = params[0];
		indices[1] = params[1];
		expand_env_var_to_res(shell, word, temp, indices);
		params[0] = indices[0];
		params[1] = indices[1];
	}
	else if (params[4] == REGULAR_CHAR)
		temp[(params[1])++] = word[(params[0])++];
}

void	process_word(t_shell *shell, char *word, char *temp, int *j)
{
	int	params[5];

	params[0] = 0;
	params[1] = *j;
	params[2] = 0;
	params[3] = 0;
	while (word[params[0]])
	{
		if ((word[params[0]] == '\'' && !params[3])
			|| (word[params[0]] == '"' && !params[2]))
		{
			params[4] = QUOTE_CHAR;
			handle_character(shell, word, temp, params);
		}
		else if ((word[params[0]] == '$') && !params[2]
			&& (ft_isalnum(word[params[0] + 1])
				|| word[params[0] + 1] == '_' || word[params[0] + 1] == '?'))
		{
			params[4] = ENV_VAR;
			handle_character(shell, word, temp, params);
		}
		else
		{
			params[4] = REGULAR_CHAR;
			handle_character(shell, word, temp, params);
		}
	}
	temp[params[1]] = '\0';
	*j = params[1];
}
