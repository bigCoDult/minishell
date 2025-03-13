/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:56:34 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 03:00:03 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_redirections(t_shell *shell, t_redirection *redirs)
{
	t_redirection	*current;
	int				last_input_fd;
	int				last_output_fd;

	if (!redirs)
		return (0);
	if (shell->heredoc.original_stdin == -1)
		shell->heredoc.original_stdin = dup(STDIN_FILENO);
	if (shell->original_stdout == -1)
		shell->original_stdout = dup(STDOUT_FILENO);
	if (shell->original_stderr == -1)
		shell->original_stderr = dup(STDERR_FILENO);
	if (shell->heredoc.original_stdin == -1
		|| shell->original_stdout == -1 || shell->original_stderr == -1)
	{
		if (shell->heredoc.original_stdin != -1)
		{
			close(shell->heredoc.original_stdin);
			shell->heredoc.original_stdin = -1;
		}
		if (shell->original_stdout != -1)
		{
			close(shell->original_stdout);
			shell->original_stdout = -1;
		}
		if (shell->original_stderr != -1)
		{
			close(shell->original_stderr);
			shell->original_stderr = -1;
		}
		return (1);
	}
	last_input_fd = -1;
	current = redirs;
	while (current)
	{
		if (current->type == REDIR_IN)
		{
			if (last_input_fd != -1)
				close(last_input_fd);
			last_input_fd = open(current->filename, O_RDONLY);
			if (last_input_fd == -1)
			{
				fprintf(stderr, "minishell: %s: No such file or directory\n", current->filename);
				restore_io(shell);
				return (1);
			}
		}
		current = current->next;
	}
	if (last_input_fd != -1)
	{
		if (dup2(last_input_fd, STDIN_FILENO) == -1)
		{
			close(last_input_fd);
			restore_io(shell);
			return (1);
		}
		close(last_input_fd);
	}
	last_output_fd = -1;
	current = redirs;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			if (last_output_fd != -1)
				close(last_output_fd);
			if (current->type == REDIR_OUT)
				last_output_fd = open(current->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				last_output_fd = open(current->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (last_output_fd == -1)
			{
				fprintf(stderr, "minishell: %s: Permission denied\n", current->filename);
				restore_io(shell);
				return (1);
			}
		}
		current = current->next;
	}
	if (last_output_fd != -1)
	{
		if (dup2(last_output_fd, STDOUT_FILENO) == -1)
		{
			close(last_output_fd);
			restore_io(shell);
			return (1);
		}
		close(last_output_fd);
	}
	return (0);
}

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

void	restore_io(t_shell *shell)
{
	if (shell->heredoc.original_stdin != -1)
	{
		if (fcntl(shell->heredoc.original_stdin, F_GETFD) != -1)
		{
			if (dup2(shell->heredoc.original_stdin, STDIN_FILENO) == -1)
				perror("dup2 error while restoring stdin");
			close(shell->heredoc.original_stdin);
		}
		shell->heredoc.original_stdin = -1;
	}
	if (shell->original_stdout != -1)
	{
		if (fcntl(shell->original_stdout, F_GETFD) != -1)
		{
			if (dup2(shell->original_stdout, STDOUT_FILENO) == -1)
				perror("dup2 error while restoring stdout");
			close(shell->original_stdout);
		}
		shell->original_stdout = -1;
	}
	if (shell->original_stderr != -1)
	{
		if (fcntl(shell->original_stderr, F_GETFD) != -1)
		{
			if (dup2(shell->original_stderr, STDERR_FILENO) == -1)
				perror("dup2 error while restoring stderr");
			close(shell->original_stderr);
		}
		shell->original_stderr = -1;
	}
	fflush(stdout);
	fflush(stderr);
}
