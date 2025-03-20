/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 08:47:31 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 03:51:59 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_single_redirection(t_redirection *current, int *last_fd)
{
	if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
	{
		if (*last_fd != -1)
			close(*last_fd);
		*last_fd = open_output_file(current);
		if (*last_fd == -1)
		{
			printf("minishell: %s: Permission denied\n", current->filename);
			return (0);
		}
	}
	return (0);
}

static int	setup_final_output(int last_fd, t_shell *shell)
{
	if (last_fd != -1)
	{
		if (dup2(last_fd, STDOUT_FILENO) == -1)
		{
			close(last_fd);
			restore_io(shell);
			return (1);
		}
		close(last_fd);
	}
	return (0);
}

int	handle_output_redirections(t_shell *shell, t_redirection *redirs)
{
	t_redirection	*current;
	int				last_fd;
	int				error_occurred;

	last_fd = -1;
	error_occurred = 0;
	current = redirs;
	while (current)
	{
		if (process_single_redirection(current, &last_fd))
		{
			error_occurred = 1;
		}
		current = current->next;
	}
	
	// 최종적으로 파일 디스크립터가 설정되었으면 설정
	if (last_fd != -1)
		return (setup_final_output(last_fd, shell));
	
	// 에러가 있었으면 에러 코드 반환
	return (error_occurred);
}
