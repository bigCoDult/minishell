/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:08:12 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 05:08:40 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(t_token *curr)
{
	int	arg_count;

	arg_count = 0;
	while (curr && curr->type != TOKEN_PIPE)
	{
		if (curr->type == TOKEN_WORD && (!curr->prev
				|| curr->prev->type != TOKEN_REDIR))
			arg_count++;
		if (curr->type == TOKEN_REDIR)
			curr = curr->next;
		if (curr)
			curr = curr->next;
	}
	return (arg_count);
}

char	**create_args_array(t_shell *shell, t_token *start, int arg_count)
{
	char	**args;
	int		i;

	args = shell_malloc(shell, sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	args[arg_count] = NULL;
	i = 0;
	while (start && start->type != TOKEN_PIPE)
	{
		if (start->type == TOKEN_WORD)
		{
			if (!start->prev || start->prev->type != TOKEN_REDIR)
				args[i++] = shell_strdup(shell, start->value);
			start = start->next;
		}
		else if (start->type == TOKEN_REDIR)
			start = start->next->next;
	}
	return (args);
}

void	add_redirection(t_redirection **redirs, t_redirection *new_redir)
{
	t_redirection	*curr;

	if (!*redirs)
	{
		*redirs = new_redir;
		return ;
	}
	curr = *redirs;
	while (curr->next)
		curr = curr->next;
	curr->next = new_redir;
}

t_redirection	*process_redirections(t_shell *shell, t_token **tokens)
{
	t_redirection	*first_redir;
	t_redirection	*new_redir;
	t_token			*curr;

	first_redir = NULL;
	curr = *tokens;
	while (curr && curr->type != TOKEN_PIPE)
	{
		if (curr->type == TOKEN_REDIR)
		{
			new_redir = create_redirection(shell, curr);
			if (!new_redir)
				return (NULL);
			add_redirection(&first_redir, new_redir);
			curr = curr->next->next;
		}
		else
			curr = curr->next;
	}
	*tokens = curr;
	return (first_redir);
}

t_command	*create_command(t_shell *shell, t_token **tokens)
{
	t_command	*cmd;
	t_token		*curr;
	int			arg_count;

	cmd = shell_malloc(shell, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	curr = *tokens;
	arg_count = count_args(curr);
	cmd->args = create_args_array(shell, curr, arg_count);
	if (!cmd->args)
		return (NULL);
	cmd->redirs = process_redirections(shell, tokens);
	return (cmd);
}
