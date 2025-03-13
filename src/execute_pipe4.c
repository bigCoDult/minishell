/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 04:45:12 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 04:45:24 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_file_descriptors(int original_stdin, int original_stdout,
								int original_stderr)
{
	if (original_stdin != -1)
		close(original_stdin);
	if (original_stdout != -1)
		close(original_stdout);
	if (original_stderr != -1)
		close(original_stderr);
}

static int	save_file_descriptors(t_shell *shell, int *original_stdin,
								int *original_stdout, int *original_stderr)
{
	*original_stdin = dup(STDIN_FILENO);
	*original_stdout = dup(STDOUT_FILENO);
	*original_stderr = dup(STDERR_FILENO);
	if (*original_stdin == -1 || *original_stdout == -1
		|| *original_stderr == -1)
	{
		close_file_descriptors(*original_stdin,
			*original_stdout, *original_stderr);
		return (1);
	}
	shell->heredoc.original_stdin = *original_stdin;
	return (0);
}

static void	restore_file_descriptors(int original_stdin, int original_stdout,
								int original_stderr)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	dup2(original_stderr, STDERR_FILENO);
	close_file_descriptors(original_stdin, original_stdout, original_stderr);
}

int	handle_all_heredocs(t_shell *shell, t_ast_node *node)
{
	int	original_stdin;
	int	original_stdout;
	int	original_stderr;
	int	result;

	if (save_file_descriptors(shell, &original_stdin, &original_stdout,
			&original_stderr))
		return (1);
	result = handle_heredocs_recursive(shell, node);
	restore_file_descriptors(original_stdin, original_stdout, original_stderr);
	return (result);
}
