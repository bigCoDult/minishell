/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 05:08:31 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 18:00:24 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_input(t_shell *shell)
{
	t_token	*curr_token;

	if (shell->tokens)
	{
		free_command_memory(shell);
		shell->tokens = NULL;
	}
	if (shell->ast_root)
	{
		free_command_memory(shell);
		shell->ast_root = NULL;
	}
	if (tokenize_input(shell) != 0)
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
