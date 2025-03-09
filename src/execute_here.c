/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:13:01 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/08 12:46:31 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_heredoc_entries(t_shell *shell)
{
	t_heredoc_entry	*current;
	t_heredoc_entry	*next;

	current = shell->heredoc.entries;
	while (current)
	{
		next = current->next;
		if (current->fd != -1)
		{
			close(current->fd);
		}
		shell_free(shell, current->delimiter);
		shell_free(shell, current);
		current = next;
	}
	shell->heredoc.entries = NULL;
	shell->heredoc.count = 0;
}

void	cleanup_heredoc(t_shell *shell)
{
	// 히어독 파일 디스크립터 닫기
	if (shell->heredoc.entries)
	{
		t_heredoc_entry *entry = shell->heredoc.entries;
		while (entry)
		{
			if (entry->fd != -1)
			{
				close(entry->fd);
				entry->fd = -1;
			}
			entry = entry->next;
		}
	}
	
	// 표준 입력 복원
	if (shell->heredoc.original_stdin != -1)
	{
		dup2(shell->heredoc.original_stdin, STDIN_FILENO);
		close(shell->heredoc.original_stdin);
		shell->heredoc.original_stdin = -1;
	}
	
	// 히어독 임시 파일 정리
	if (shell->heredoc.temp_file)
	{
		unlink(shell->heredoc.temp_file);
		free(shell->heredoc.temp_file);
		shell->heredoc.temp_file = NULL;
	}
}

int	find_command_heredoc_fd(t_shell *shell, t_command *cmd)
{
	t_redirection	*redir;
	t_heredoc_entry	*entry;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			entry = shell->heredoc.entries;
			while (entry)
			{
				if (ft_strcmp(entry->delimiter, redir->filename) == 0)
					return (entry->fd);
				entry = entry->next;
			}
		}
		redir = redir->next;
	}
	return (-1);
}

void	setup_command_heredoc(t_shell *shell, t_command *cmd)
{
	int	heredoc_fd;

	heredoc_fd = find_command_heredoc_fd(shell, cmd);
	if (heredoc_fd != -1)
	{
		close(heredoc_fd);
		heredoc_fd = open(shell->heredoc.temp_file, O_RDONLY);
		dup2(heredoc_fd, STDIN_FILENO);
	}
}
