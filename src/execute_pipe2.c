/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 04:32:17 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 18:50:50 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_left_command(t_shell *shell, t_ast_node *node, int pipefd[2])
{
	int	ret;

	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
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
	free_exit(shell, 1);
}

void	execute_external_command(t_shell *shell, t_command *cmd)
{
	char	*path;

	path = find_command_path(shell, cmd->args[0]);
	if (!path)
		free_exit(shell, 127);
	execve(path, cmd->args, get_env_array(shell));
	free_exit(shell, 127);
}

void	execute_right_command(t_shell *shell, t_ast_node *node, int pipefd[2])
{
	int	ret;

	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	if (node->right->type == AST_PIPE)
	{
		ret = execute_pipe(shell, node->right);
		free_exit(shell, ret);
	}
	else if (node->right->type == AST_COMMAND)
	{
		setup_right_command_io(shell, node->right->cmd);
		if (is_builtin(node->right->cmd->args[0]))
		{
			ret = execute_builtin(shell, node->right->cmd);
			free_exit(shell, ret);
		}
		else
			execute_external_command(shell, node->right->cmd);
	}
	free_exit(shell, 1);
}

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

int	execute_pipe(t_shell *shell, t_ast_node *node)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

	if (!node || !node->left || !node->right)
		return (1);
	if (handle_all_heredocs(shell, node) != 0)
		return (1);
	if (pipe(pipefd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 == 0)
		execute_left_command(shell, node, pipefd);
	pid2 = fork();
	if (pid2 == 0)
		execute_right_command(shell, node, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	return (WEXITSTATUS(status2));
}
