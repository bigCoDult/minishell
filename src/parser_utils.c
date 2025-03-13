/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 05:08:31 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 02:52:56 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_tokens(t_shell *shell)
{
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
