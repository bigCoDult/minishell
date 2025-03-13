/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:56:34 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 08:35:59 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirection	*create_redirection(t_shell *shell, t_token *token)
{
	t_redirection	*redir;

	redir = shell_malloc(shell, sizeof(t_redirection));
	if (!redir)
		return (NULL);
	if (ft_strcmp(token->value, "<<") == 0)
		redir->type = REDIR_HEREDOC;
	else if (ft_strcmp(token->value, "<") == 0)
		redir->type = REDIR_IN;
	else if (ft_strcmp(token->value, ">") == 0)
		redir->type = REDIR_OUT;
	else if (ft_strcmp(token->value, ">>") == 0)
		redir->type = REDIR_APPEND;
	redir->filename = shell_strdup(shell, token->next->value);
	if (!redir->filename)
	{
		shell_free(shell, redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}
