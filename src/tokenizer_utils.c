/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 06:04:30 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/25 11:42:12 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_token_state
{
	int	in_single_quote;
	int	in_double_quote;
}	t_token_state;

static int	get_word_length(char *input)
{
	int				len;
	t_token_state	state;
	int				dollar_sign;

	debug_print(0, 3, "DEBUG: Calculating word length\n");
	len = 0;
	state.in_single_quote = 0;
	state.in_double_quote = 0;
	dollar_sign = 0;
	while (input[len])
	{
		if (input[len] == '$' && !state.in_single_quote)
			dollar_sign = 1;
		else if (input[len] == '"' && !state.in_single_quote && !dollar_sign)
		{
			state.in_double_quote = !state.in_double_quote;
			len++;
			continue ;
		}
		else if (input[len] == '\'' && !state.in_double_quote && !dollar_sign)
		{
			state.in_single_quote = !state.in_single_quote;
			len++;
			continue ;
		}
		else
			dollar_sign = 0;
		if (!state.in_single_quote && !state.in_double_quote
			&& strchr(" |<>", input[len]))
			break ;
		debug_print(0, 3, "DEBUG: Checking character: %c\n", input[len]);
		len++;
	}
	if (state.in_single_quote || state.in_double_quote)
	{
		debug_print(0, 3, "DEBUG: Unclosed quote detected\n");
	}
	debug_print(0, 3, "DEBUG: Word length: %d\n", len);
	return (len);
}

char	*handle_word(t_shell *shell, char *input, int *len)
{
	char			*word;
	char			*result;
	t_token_state	state;
	int				i;
	int				j;
	int				final_quote_state;
	int				dollar_sign;

	final_quote_state = 0;
	debug_print(0, 3, "DEBUG: Handling word starting with: %c\n", *input);
	*len = get_word_length(input);
	if (*len == 0)
		return (NULL);
	word = shell_malloc(shell, *len + 1);
	if (!word)
		return (NULL);
	state.in_single_quote = 0;
	state.in_double_quote = 0;
	i = 0;
	j = 0;
	if (input[0] == '\'')
		final_quote_state = 1;
	else if (input[0] == '"' && input[0] != '$')
		final_quote_state = 2;
	else if (input[0] == '$' && input[1] == '"')
	{
		final_quote_state = 3;
		i += 2;
	}
	else if (input[0] == '$' && input[1] == '\'')
	{
		final_quote_state = 4;
		i += 2;
	}
	while (i < *len)
	{
		if (input[i] == '$' && !state.in_single_quote)
			dollar_sign = 1;
		else if (input[i] == '"' && !state.in_single_quote && !dollar_sign)
		{
			state.in_double_quote = !state.in_double_quote;
			i++;
			continue ;
		}
		else if (input[i] == '\'' && !state.in_double_quote && !dollar_sign)
		{
			state.in_single_quote = !state.in_single_quote;
			i++;
			continue ;
		}
		else
			dollar_sign = 0;
		word[j++] = input[i++];
	}
	word[j] = '\0';
	debug_print(0, 3, "DEBUG: Quote state: %d\n", final_quote_state);
	if (final_quote_state != 1 && strncmp(word, "$\"", 2) != 0)
		result = expand_env_var(shell, word);
	else
		result = shell_strdup(shell, word);
	shell_free(shell, word);
	debug_print(0, 3, "DEBUG: Expanded word: %s\n", result);
	return (result);
}
