/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 07:52:12 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 08:10:45 by yutsong          ###   ########.fr       */
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

static void	initialize_params(int params[5], int *j)
{
	params[0] = 0;
	params[1] = *j;
	params[2] = 0;
	params[3] = 0;
	params[4] = 0;
}

static void	set_char_type(char *word, int params[5])
{
	if ((word[params[0]] == '\'' && !params[3])
		|| (word[params[0]] == '"' && !params[2]))
		params[4] = QUOTE_CHAR;
	else if ((word[params[0]] == '$') && !params[2]
		&& (ft_isalnum(word[params[0] + 1])
			|| word[params[0] + 1] == '_'
			|| word[params[0] + 1] == '?'))
		params[4] = ENV_VAR;
	else
		params[4] = REGULAR_CHAR;
}

void	process_word(t_shell *shell, char *word, char *temp, int *j)
{
	int	params[5];

	initialize_params(params, j);
	while (word[params[0]])
	{
		set_char_type(word, params);
		handle_character(shell, word, temp, params);
	}
	temp[params[1]] = '\0';
	*j = params[1];
}
