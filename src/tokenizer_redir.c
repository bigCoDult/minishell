/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:15:26 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 05:56:18 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_red_token(t_shell *shell, const char *value, t_token_type type)
{
	t_token	*token;

	token = shell_malloc(shell, sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = shell_strdup(shell, value);
	if (!token->value)
	{
		shell_free(shell, token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

t_token	*create_word_token(t_shell *shell, char **input, t_token *token)
{
	t_token	*delimiter_token;
	char	*word;
	int		word_len;

	while (**input && ft_isspace(**input))
		(*input)++;
	if (!**input || **input == '|' || **input == '<' || **input == '>')
	{
		if (**input)
			printf(
				"minishell: syntax error near unexpected token `%c'\n", **input);
		else
			printf("minishell: syntax error near unexpected token `%c'\n", '\n');
		shell->status.exit_code = 258;
		shell_free(shell, token);
		return (NULL);
	}
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

t_token	*handle_heredoc_redirection(t_shell *shell, char **input)
{
	t_token	*token;

	token = create_red_token(shell, "<<", TOKEN_REDIR);
	if (!token)
		return (NULL);
	*input += 2;
	return (create_word_token(shell, input, token));
}

t_token	*handle_regular_redirection(t_shell *shell, char **input)
{
	t_token	*token;
	char	redir[2];

	if (**input == '>' && *(*input + 1) == '>')
	{
		token = create_red_token(shell, ">>", TOKEN_REDIR);
		*input += 2;
	}
	else
	{
		redir[0] = **input;
		redir[1] = '\0';
		token = create_red_token(shell, redir, TOKEN_REDIR);
		(*input)++;
	}
	if (!token)
		return (NULL);
	return (create_word_token(shell, input, token));
}

t_token	*handle_redirection(t_shell *shell, char **input)
{
	if (**input == '<' && *(*input + 1) == '<')
		return (handle_heredoc_redirection(shell, input));
	else
		return (handle_regular_redirection(shell, input));
}
