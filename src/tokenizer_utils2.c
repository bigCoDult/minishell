/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 05:58:23 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 13:42:40 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_quote_state(char *word, int *i, int *in_single_quote,
						int *in_double_quote)
{
	if (word[*i] == '\'' && !(*in_double_quote))
	{
		*in_single_quote = !(*in_single_quote);
		(*i)++;
	}
	else if (word[*i] == '"' && !(*in_single_quote))
	{
		*in_double_quote = !(*in_double_quote);
		(*i)++;
	}
	else
		(*i)++;
}

static char	*extract_var_name_tok(t_shell *shell, char *word, int *i)
{
	int		var_start;
	char	*var_name;

	(*i)++;
	var_start = *i;
	while (word[*i] && (ft_isalnum(word[*i])
			|| word[*i] == '_' || word[*i] == '?'))
	{
		if (word[*i] == '?')
		{
			(*i)++;
			break ;
		}
		(*i)++;
	}
	var_name = shell_malloc(shell, *i - var_start + 1);
	ft_strncpy(var_name, word + var_start, *i - var_start);
	var_name[*i - var_start] = '\0';
	return (var_name);
}

void	expand_env_var_to_res(t_shell *shell,
		char *word, char *temp, int indices[2])
{
	char	*var_name;
	char	*var_value;
	int		var_len;
	char	status[16];

	var_name = extract_var_name_tok(shell, word, &indices[0]);
	if (ft_strcmp(var_name, "?") == 0)
	{
		ft_itoa_n(status, sizeof(status), shell->status.exit_code);
		var_value = status;
	}
	else
		var_value = get_env_value(shell, var_name);
	if (var_value)
	{
		var_len = ft_strlen(var_value);
		ft_strcpy(temp + indices[1], var_value);
		indices[1] += var_len;
	}
	shell_free(shell, var_name);
}

char	*finalize_word(t_shell *shell, char *word, int quote_state)
{
	char	*result;
	char	*temp;
	int		j;

	(void)quote_state;
	temp = shell_malloc(shell, ft_strlen(word) * 2);
	if (!temp)
		return (NULL);
	j = 0;
	process_word(shell, word, temp, &j);
	result = shell_strdup(shell, temp);
	shell_free(shell, temp);
	return (result);
}

char	*handle_word(t_shell *shell, char *input, int *len)
{
	char	*word;
	char	*result;
	int		quote_state;
	int		start_idx;

	*len = get_word_length(input);
	if (*len == 0)
		return (NULL);
	word = shell_malloc(shell, *len + 1);
	if (!word)
		return (NULL);
	quote_state = determine_quote_state(input, &start_idx);
	process_word_content(input, word, *len);
	result = finalize_word(shell, word, quote_state);
	shell_free(shell, word);
	return (result);
}
