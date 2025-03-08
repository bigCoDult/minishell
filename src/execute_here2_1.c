/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here2_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 03:00:36 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/08 15:50:41 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_null_line(t_shell *shell, int fd)
{
	free_env(shell);
	free_shell_malloc(shell);
	close(fd);
	if (g_signal == SIGINT)
		exit(130);
	else
		exit(0);
}

void	check_delimiter_match(char *line, char *delimiter, int fd, t_shell *shell)
{
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		close(fd);
		// exit(0);
		free_exit(shell, 0);
	}
}

void	process_heredoc_lines(t_shell *shell, char *delimiter, int fd)
{
	char	*line;
	int		original_stdout;
	int		original_stderr;

	(void)shell;
	check_heredoc_signal(fd);
	
	// 표준 출력과 표준 에러 복제
	original_stdout = dup(STDOUT_FILENO);
	original_stderr = dup(STDERR_FILENO);
	if (original_stdout == -1 || original_stderr == -1)
	{
		if (original_stdout != -1)
			close(original_stdout);
		if (original_stderr != -1)
			close(original_stderr);
		handle_null_line(shell, fd);
	}
	
	// 표준 출력을 표준 에러로 리디렉션하지 않고, 프롬프트 직접 출력
	while (1)
	{
		check_heredoc_signal(fd);
		
		// 프롬프트를 표준 에러로 직접 출력
		write(STDERR_FILENO, "> ", 2);
		
		// readline을 사용하되 빈 프롬프트로 설정 (이미 직접 출력했으므로)
		line = readline("");
		
		if (!line)
		{
			close(original_stdout);
			close(original_stderr);
			handle_null_line(shell, fd);
		}
		
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			close(original_stdout);
			close(original_stderr);
			close(fd);
			free_exit(shell, 0);
		}
		
		// 히어독 내용 파일에 쓰기
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		
		free(line);
	}
	
	// 여기까지 도달할 일은 없음
	close(original_stdout);
	close(original_stderr);
}
