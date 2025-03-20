/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 06:28:37 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 14:32:23 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_right_sub(t_redirection *redir, t_heredoc_entry *entry)
{
	int	heredoc_fd;

	while (entry)
	{
		if (ft_strcmp(entry->delimiter, redir->filename) == 0)
		{
			if (entry->fd != -1)
				close(entry->fd);
			heredoc_fd = open(entry->temp_file, O_RDONLY);
			if (heredoc_fd != -1)
			{
				dup2(heredoc_fd, STDIN_FILENO);
				close(heredoc_fd);
			}
			break ;
		}
		entry = entry->next;
	}
}

void	setup_right_command_io(t_shell *shell, t_command *cmd)
{
	t_redirection	*redir;
	t_heredoc_entry	*entry;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			entry = shell->heredoc.entries;
			setup_right_sub(redir, entry);
		}
		redir = redir->next;
	}
	if (setup_redirections(shell, cmd->redirs) != 0)
		free_exit(shell, 1);
}

void	execute_left_command(t_shell *shell, t_ast_node *node, int pipefd[2])
{
	int	ret;

	close(pipefd[0]);
	if (node->left->type == AST_COMMAND)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 error in left command");
			close(pipefd[1]);
			free_exit(shell, 1);
		}
		close(pipefd[1]);
		if (setup_redirections(shell, node->left->cmd->redirs) != 0)
			free_exit(shell, 1);
		if (node->left->cmd->args
			&& node->left->cmd->args[0] && is_builtin(node->left->cmd->args[0]))
		{
			ret = execute_builtin(shell, node->left->cmd);
			free_exit(shell, ret);
		}
		else if (node->left->cmd->args && node->left->cmd->args[0])
			execute_external_command(shell, node->left->cmd);
		else
			free_exit(shell, 0);
	}
	close(pipefd[1]);
	write(STDERR_FILENO, "minishell: left command error\n", 30);
	free_exit(shell, 1);
}
