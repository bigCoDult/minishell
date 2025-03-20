/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_redir2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 05:53:44 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 07:13:30 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_input_validity(t_shell *shell, char **input, t_token *token)
{
	if (!**input || **input == '|' || **input == '<' || **input == '>')
	{
		if (**input)
			printf(
				"minishell: syntax error near unexpected token `%c'\n",
				**input);
		else
			printf(
				"minishell: syntax error near unexpected token `%c'\n", '\n');
		shell->status.exit_code = 258;
		shell_free(shell, token);
		return (1);
	}
	return (0);
}

static t_token	*create_and_add_token(
	t_shell *shell, char **input, t_token *token)
{
	t_token	*delimiter_token;
	char	*word;
	int		word_len;

	word = handle_word(shell, *input, &word_len);
	if (!word)
	{
		shell_free(shell, token);
		return (NULL);
	}
	delimiter_token = create_token(shell, word, TOKEN_WORD);
	shell_free(shell, word);
	if (!delimiter_token)
	{
		shell_free(shell, token);
		return (NULL);
	}
	*input += word_len;
	add_token(shell, token);
	add_token(shell, delimiter_token);
	return (token);
}

t_token	*create_word_token(t_shell *shell, char **input, t_token *token)
{
	while (**input && ft_isspace(**input))
		(*input)++;
	if (check_input_validity(shell, input, token))
		return (NULL);
	return (create_and_add_token(shell, input, token));
}
