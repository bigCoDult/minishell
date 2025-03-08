/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:31:16 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/08 15:48:58 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredocs_recursive(t_shell *shell, t_ast_node *node)
{
	t_redirection	*redir;

	if (!node)
		return (0);
	if (node->type == AST_COMMAND)
	{
		redir = node->cmd->redirs;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				if (handle_heredoc(shell, redir->filename) != 0)
					return (1);
			}
			redir = redir->next;
		}
		return (0);
	}
	if (node->type == AST_PIPE)
	{
		if (handle_heredocs_recursive(shell, node->left) != 0)
			return (1);
		return (handle_heredocs_recursive(shell, node->right));
	}
	return (0);
}

void	setup_pipe(t_shell *shell, int pipefd[2], int is_first, int is_last)
{
	if (!is_first)
	{
		dup2(shell->pipe_info.prev_pipe, STDIN_FILENO);
		close(shell->pipe_info.prev_pipe);
	}
	if (!is_last)
		dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
}

void	execute_in_pipe(t_shell *shell, t_command *cmd)
{
	char	*executable_path;
	char	**env_array;

	if (shell->pipe_info.prev_pipe != -1)
	{
		dup2(shell->pipe_info.prev_pipe, STDIN_FILENO);
		close(shell->pipe_info.prev_pipe);
	}
	if (!shell->pipe_info.is_last)
		dup2(shell->pipe_info.pipefd[1], STDOUT_FILENO);
	close(shell->pipe_info.pipefd[0]);
	close(shell->pipe_info.pipefd[1]);
	executable_path = find_command_path(shell, cmd->args[0]);
	if (!executable_path)
		free_exit(shell, 127);
	env_array = get_env_array(shell);
	if (!env_array)
		free_exit(shell, 1);
	execve(executable_path, cmd->args, env_array);
	free_exit(shell, 127);
}

int	execute_piped_command(t_shell *shell, t_command *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execute_in_pipe(shell, cmd);
	if (shell->pipe_info.prev_pipe != -1)
		close(shell->pipe_info.prev_pipe);
	close(shell->pipe_info.pipefd[1]);
	return (pid);
}

int	handle_all_heredocs(t_shell *shell, t_ast_node *node)
{
	int	original_stdin;
	int	original_stdout;
	int	original_stderr;
	int	result;

	// 모든 표준 입출력 파일 디스크립터 저장
	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	original_stderr = dup(STDERR_FILENO);
	
	if (original_stdin == -1 || original_stdout == -1 || original_stderr == -1)
	{
		if (original_stdin != -1)
			close(original_stdin);
		if (original_stdout != -1)
			close(original_stdout);
		if (original_stderr != -1)
			close(original_stderr);
		return (1);
	}
	
	shell->heredoc.original_stdin = original_stdin;
	result = handle_heredocs_recursive(shell, node);
	
	// 모든 표준 입출력 복원
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	dup2(original_stderr, STDERR_FILENO);
	
	close(original_stdin);
	close(original_stdout);
	close(original_stderr);
	
	return (result);
}
