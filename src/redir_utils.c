/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:56:34 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/07 12:59:51 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_input_redirect(t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		printf("minishell: %s: No such file or directory\n", redir->filename);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	setup_output_redirect(t_redirection *redir)
{
	int	fd;
	int	flags;

	if (redir->type == REDIR_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(redir->filename, flags, 0644);
	if (fd == -1)
	{
		printf("minishell: %s: Permission denied\n", redir->filename);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	setup_redirections(t_shell *shell, t_redirection *redirs)
{
	t_redirection	*current;

	if (!redirs)
		return (0);  // 리다이렉션이 없으면 바로 반환

	// 표준 입출력 백업 - 이미 백업되지 않은 경우에만
	if (shell->heredoc.original_stdin == -1)
		shell->heredoc.original_stdin = dup(STDIN_FILENO);
	if (shell->original_stdout == -1)
		shell->original_stdout = dup(STDOUT_FILENO);
	
	// 백업 실패 시 안전하게 처리
	if (shell->heredoc.original_stdin == -1 || shell->original_stdout == -1)
	{
		// 백업된 디스크립터만 닫기
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
		return (1);
	}

	// 모든 리다이렉션 처리
	current = redirs;
	while (current)
	{
		if (current->type == REDIR_IN)
		{
			if (setup_input_redirect(current) != 0)
			{
				restore_io(shell);  // 오류 발생 시 원상태로 복원
				return (1);
			}
		}
		else if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			if (setup_output_redirect(current) != 0)
			{
				restore_io(shell);  // 오류 발생 시 원상태로 복원
				return (1);
			}
		}
		current = current->next;
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
	// 표준 입력 복원
	if (shell->heredoc.original_stdin != -1)
	{
		dup2(shell->heredoc.original_stdin, STDIN_FILENO);
		close(shell->heredoc.original_stdin);
		shell->heredoc.original_stdin = -1;
	}
	
	// 표준 출력 복원
	if (shell->original_stdout != -1)
	{
		dup2(shell->original_stdout, STDOUT_FILENO);
		close(shell->original_stdout);
		shell->original_stdout = -1;
	}
	
	// 터미널 버퍼 제어 코드 완전히 제거
}
