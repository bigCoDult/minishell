/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:07:22 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 20:28:57 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(t_shell *shell, char *value, t_token_type type)
{
	t_token	*token;

	token = shell_malloc(shell, sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = shell_strdup(shell, value);
	if (!token->value)
		return (NULL);
	token->type = type;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

void	add_token(t_shell *shell, t_token *token)
{
	t_token	*current;

	if (!shell->tokens)
		shell->tokens = token;
	else
	{
		current = shell->tokens;
		while (current->next)
			current = current->next;
		current->next = token;
		token->prev = current;
	}
	current = shell->tokens;
	while (current)
		current = current->next;
}

int	handle_pipe_token(t_shell *shell, char **input)
{
	t_token	*token;

	token = create_token(shell, "|", TOKEN_PIPE);
	if (!token)
		return (1);
	add_token(shell, token);
	(*input)++;
	return (0);
}

int	handle_word_token(t_shell *shell, char **input)
{
	t_token	*token;
	char	*word;
	int		word_len;

	word = handle_word(shell, *input, &word_len);
	if (!word)
		return (1);
	token = create_token(shell, word, TOKEN_WORD);
	shell_free(shell, word);
	if (!token)
		return (1);
	add_token(shell, token);
	*input += word_len;
	return (0);
}

char	*skip_spaces(char *input)
{
	while (*input
		&& (ft_isspace(*input)
			|| *input == '(' || *input == ')' || *input == ';'))
		input++;
	return (input);
}
