/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 05:08:31 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 14:29:18 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_tokens(t_shell *shell)
{
	t_token	*current;
	t_token	*last_token;

	if (!shell->tokens)
		return (0);
	last_token = shell->tokens;
	while (last_token->next)
		last_token = last_token->next;
	if (last_token->type == TOKEN_PIPE)
	{
		write(STDERR_FILENO,
			"minishell: syntax error near unexpected token '|'\n", 51);
		shell->status.exit_code = 2;
		return (1);
	}
	else if (last_token->type == TOKEN_REDIR)
	{
		write(STDERR_FILENO,
			"minishell: syntax error near unexpected token `newline'\n", 56);
		shell->status.exit_code = 2;
		return (1);
	}
	current = shell->tokens;
	while (current && current->next)
	{
		if (current->type == TOKEN_REDIR)
		{
			if (current->next->type != TOKEN_WORD)
			{
				printf("minishell: syntax error near unexpected token `%s'\n",
					current->next->value);
				shell->status.exit_code = 2;
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}

int	parse_input(t_shell *shell)
{
	t_token	*curr_token;

	if (shell->tokens)
	{
		free_non_heredoc_memory(shell);
		shell->tokens = NULL;
	}
	if (shell->ast_root)
	{
		free_non_heredoc_memory(shell);
		shell->ast_root = NULL;
	}
	if (tokenize_input(shell) != 0)
		return (1);
	if (validate_tokens(shell) != 0)
		return (1);
	curr_token = shell->tokens;
	while (curr_token)
		curr_token = curr_token->next;
	curr_token = shell->tokens;
	shell->ast_root = parse_pipeline(shell, &curr_token);
	if (!shell->ast_root)
		return (1);
	return (0);
}
