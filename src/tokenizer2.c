/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 06:05:33 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 01:39:27 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_next_token(t_shell *shell, char **input)
{
	t_token	*token;
	int		is_first_token;

	is_first_token = (shell->tokens == NULL);
	
	if (**input == '|')
	{
		if (is_first_token)
		{
			fprintf(stderr, "minishell: syntax error near unexpected token '|'\n");
			shell->status.exit_code = 258;
			return (1);
		}
		if (handle_pipe_token(shell, input))
			return (1);
	}
	else if (**input == '<' || **input == '>')
	{
		token = handle_redirection(shell, input);
		if (!token)
			return (1);
	}
	else
	{
		if (handle_word_token(shell, input))
			return (1);
	}
	return (0);
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
