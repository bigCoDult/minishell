/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 06:05:33 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 18:03:17 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_first_pipe_token(t_shell *shell)
{
	write(STDERR_FILENO,
		"minishell: syntax error near unexpected token '|'\n", 51);
	shell->status.exit_code = 1;
	return (1);
}

static int	process_pipe_token(t_shell *shell, char **input)
{
	int	is_first_token;

	is_first_token = (shell->tokens == NULL);
	if (is_first_token)
		return (handle_first_pipe_token(shell));
	return (handle_pipe_token(shell, input));
}

static int	process_redirection_token(t_shell *shell, char **input)
{
	t_token	*token;

	token = handle_redirection(shell, input);
	if (!token)
		return (1);
	return (0);
}

int	process_next_token(t_shell *shell, char **input)
{
	if (**input == '|')
		return (process_pipe_token(shell, input));
	else if (**input == '<' || **input == '>')
		return (process_redirection_token(shell, input));
	else
		return (handle_word_token(shell, input));
}

int	tokenize_input(t_shell *shell)
{
	char	*input;

	input = shell->input_line;
	while (*input)
	{
		input = skip_spaces(input);
		if (!*input)
			break ;
		if (process_next_token(shell, &input))
			return (1);
	}
	return (0);
}
