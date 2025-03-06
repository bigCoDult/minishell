/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 06:05:33 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 06:05:44 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_next_token(t_shell *shell, char **input)
{
	t_token	*token;

	if (**input == '|')
	{
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
