/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 06:28:37 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 06:50:44 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_right_command_io(t_shell *shell, t_command *cmd)
{
	int	heredoc_fd;

	heredoc_fd = find_command_heredoc_fd(shell, cmd);
	if (heredoc_fd != -1)
	{
		close(heredoc_fd);
		heredoc_fd = open(shell->heredoc.temp_file, O_RDONLY);
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
	}
	if (setup_redirections(shell, cmd->redirs) != 0)
		free_exit(shell, 1);
}

void	execute_left_command(t_shell *shell, t_ast_node *node, int pipefd[2])
{
	int	ret;

	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2 error in left command");
		close(pipefd[1]);
		free_exit(shell, 1);
	}
	close(pipefd[1]);
	if (node->left->type == AST_COMMAND)
	{
		setup_left_command_io(shell, node->left->cmd);
		if (is_builtin(node->left->cmd->args[0]))
		{
			ret = execute_builtin(shell, node->left->cmd);
			free_exit(shell, ret);
		}
		else
			execute_external_command(shell, node->left->cmd);
	}
	write(STDERR_FILENO, "minishell: left command error\n", 30);
	free_exit(shell, 1);
}
