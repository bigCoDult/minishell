/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 05:08:31 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 01:38:44 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_tokens(t_shell *shell)
{
	t_token	*last_token;
	
	if (!shell->tokens)
		return (0);
		
	// 마지막 토큰 찾기
	last_token = shell->tokens;
	while (last_token->next)
		last_token = last_token->next;
		
	// 마지막 토큰이 파이프인 경우 구문 오류
	if (last_token->type == TOKEN_PIPE)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token '|'\n");
		shell->status.exit_code = 258;
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
		
	// 토큰 유효성 검사
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
