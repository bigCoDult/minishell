/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:15:26 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 07:12:13 by yutsong          ###   ########.fr       */
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
