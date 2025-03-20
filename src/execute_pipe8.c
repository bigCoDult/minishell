/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe8.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 07:14:33 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 07:25:05 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect_to_dev_null(void)
{
	int	dev_null;

	dev_null = open("/dev/null", O_WRONLY);
	if (dev_null != -1)
	{
		dup2(dev_null, STDERR_FILENO);
		close(dev_null);
	}
	return (0);
}

int	check_left_redirections(t_shell *shell, t_ast_node *node)
{
	int	check_redir_result;
	int	status;

	check_redir_result = fork();
	if (check_redir_result == 0)
	{
		redirect_to_dev_null();
		if (setup_redirections(shell, node->left->cmd->redirs) != 0)
			free_exit(shell, 1);
		free_exit(shell, 0);
	}
	waitpid(check_redir_result, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return (1);
	return (0);
}

void	check_input_file(t_redirection *redir)
{
	int	file_fd;

	if (redir->type == REDIR_IN)
	{
		file_fd = open(redir->filename, O_RDONLY);
		if (file_fd == -1)
		{
			write(STDERR_FILENO, "minishell: ", 11);
			write(STDERR_FILENO, redir->filename, ft_strlen(redir->filename));
			write(STDERR_FILENO, ": No such file or directory\n", 28);
		}
		else
			close(file_fd);
	}
}

void	check_right_redirections(t_shell *shell, t_ast_node *node)
{
	int				check_redir_result;
	t_redirection	*redir;

	check_redir_result = fork();
	if (check_redir_result == 0)
	{
		redir = node->right->cmd->redirs;
		while (redir)
		{
			check_input_file(redir);
			redir = redir->next;
		}
		free_exit(shell, 0);
	}
	waitpid(check_redir_result, NULL, 0);
}

void	print_left_error(t_shell *shell, t_ast_node *node)
{
	int	check_redir_result;

	check_redir_result = fork();
	if (check_redir_result == 0)
	{
		setup_redirections(shell, node->left->cmd->redirs);
		free_exit(shell, 1);
	}
	waitpid(check_redir_result, NULL, 0);
}
