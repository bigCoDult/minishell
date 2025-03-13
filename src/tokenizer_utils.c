/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 06:04:30 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 04:10:50 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quote_character(char c, t_token_state *state, int *dollar_sign)
{
	int	skip_char;

	skip_char = 0;
	if (c == '\'' && !state->in_double_quote)
	{
		state->in_single_quote = !state->in_single_quote;
		skip_char = 1;
	}
	else if (c == '"' && !state->in_single_quote)
	{
		state->in_double_quote = !state->in_double_quote;
		skip_char = 1;
	}
	if (c == '$' && !state->in_single_quote)
		*dollar_sign = 1;
	else if (c != '$')
		*dollar_sign = 0;
	return (skip_char);
}

int	is_word_delimiter(char c, t_token_state state)
{
	if (!state.in_single_quote && !state.in_double_quote
		&& (ft_isspace(c) || c == '|' || c == '<' || c == '>'))
		return (1);
	return (0);
}

int	get_word_length(char *input)
{
	int				len;
	int				dollar_sign;
	t_token_state	state;

	len = 0;
	state.in_single_quote = 0;
	state.in_double_quote = 0;
	dollar_sign = 0;
	while (input[len])
	{
		if (handle_quote_character(input[len], &state, &dollar_sign))
		{
			len++;
			continue ;
		}
		if (is_word_delimiter(input[len], state))
			break ;
		len++;
	}

	return (len);
}

int	determine_quote_state(char *input, int *start_idx)
{
	int	quote_state;

	*start_idx = 0;
	if (input[0] == '$' && (input[1] == '\'' || input[1] == '"'))
	{
		*start_idx = 1;
		if (input[1] == '\'')
			quote_state = 1;
		else
			quote_state = 2;
	}
	else if (input[0] == '\'')
		quote_state = 1;
	else if (input[0] == '"' && input[0] != '$')
		quote_state = 2;
	else
		quote_state = 0;
	return (quote_state);
}

void	process_word_content(char *input, char *word, int len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;

	while (i < len)
	{
		if (input[i] == '$' && (input[i+1] == '\'' || input[i+1] == '"') && i+1 < len)
			i++;
		word[j++] = input[i++];
	}
	word[j] = '\0';
}
