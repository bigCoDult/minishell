/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:08:12 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/25 11:42:12 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*create_command(t_shell *shell, t_token **tokens)
{
	t_command		*cmd;
	t_token			*curr;
	int				arg_count;
	t_redirection	*last_redir;
	t_redirection	*redir;

	debug_print(2047, 7, "\n=== CREATE COMMAND ===\n");
	arg_count = 0;
	last_redir = NULL;
	cmd = shell_malloc(shell, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	curr = *tokens;
	while (curr && curr->type != TOKEN_PIPE)
	{
		if (curr->type == TOKEN_WORD && 
			(!curr->prev || curr->prev->type != TOKEN_REDIR))
			arg_count++;
		if (curr->type == TOKEN_REDIR)
			curr = curr->next;
		if (curr)
			curr = curr->next;
	}
	cmd->args = shell_malloc(shell, sizeof(char *) * (arg_count + 1));
	cmd->args[arg_count] = NULL;
	curr = *tokens;
	arg_count = 0;
	while (curr && curr->type != TOKEN_PIPE)
	{
		if (curr->type == TOKEN_WORD)
		{
			if (!curr->prev || curr->prev->type != TOKEN_REDIR)
			{
				debug_print(2047, 7, "Processing word: %s\n", curr->value);
				cmd->args[arg_count++] = shell_strdup(shell, curr->value);
			}
			curr = curr->next;
		}
		else if (curr->type == TOKEN_REDIR)
		{
			redir = create_redirection(shell, curr);
			if (!redir)
				return (NULL);
			if (!cmd->redirs)
				cmd->redirs = redir;
			else
				last_redir->next = redir;
			last_redir = redir;
			curr = curr->next->next;
		}
	}
	*tokens = curr;
	return (cmd);
}

char	**create_args_array(t_shell *shell, t_token *start, int arg_count)
{
	char	**args;
	int		i;

	debug_print(2047, 3, "DEBUG: [create_args_array] Creating args array with count: %d\n", arg_count);
	args = shell_malloc(shell, sizeof(char *) * (arg_count + 1));
	if (!args)
	{
		debug_print(2047, 3, "DEBUG: [create_args_array] Failed to allocate args array\n");
		return (NULL);
	}
	i = 0;
	while (start && start->type == TOKEN_WORD && i < arg_count)
	{
		debug_print(2047, 3, "DEBUG: [create_args_array] Adding arg[%d]: %s\n", i, start->value);
		args[i] = shell_strdup(shell, start->value);
		if (!args[i])
		{
			debug_print(2047, 3, "DEBUG: [create_args_array] Failed to duplicate argument\n");
			return (NULL);
		}
		start = start->next;
		i++;
	}
	args[i] = NULL;
	debug_print(2047, 3, "DEBUG: [create_args_array] Args array created successfully\n");
	return (args);
}

int	parse_input(t_shell *shell)
{
	t_token	*curr_token;

	debug_print(2047, 3, "\nDEBUG: [parse_input] === Starting input parsing ===\n");
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
	{
		debug_print(2047, 3, "DEBUG: [parse_input] Tokenization failed\n");
		return (1);
	}
	debug_print(2047, 3, "DEBUG: [parse_input] Tokenization completed\n");
	debug_print(2047, 3, "\nDEBUG: [parse_input] Token list:\n");
	curr_token = shell->tokens;
	while (curr_token)
	{
		debug_print(2047, 3, "DEBUG: [parse_input] Token type: %d, value: %s\n",
			   curr_token->type, curr_token->value);
		curr_token = curr_token->next;
	}
	curr_token = shell->tokens;
	shell->ast_root = parse_pipeline(shell, &curr_token);
	if (!shell->ast_root)
	{
		debug_print(2047, 3, "DEBUG: [parse_input] Pipeline parsing failed\n");
		return (1);
	}
	debug_print(2047, 4, "\nDEBUG: === AST Structure ===\n");
	print_ast(shell->ast_root, 0);
	debug_print(2047, 4, "DEBUG: === End of AST ===\n\n");
	debug_print(2047, 3, "DEBUG: [parse_input] Parsing completed successfully\n");
	debug_print(2047, 3, "DEBUG: [parse_input] === Parsing finished ===\n\n");
	return (0);
}
