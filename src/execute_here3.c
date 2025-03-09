/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:02:34 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/09 12:31:10 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_heredoc_content(t_shell *shell, char *delimiter, int fd)
{
	pid_t	pid;
	int		status;
	int		original_g_signal;

	status = 0;
	original_g_signal = g_signal;
	g_signal = 0;
	setup_signals_heredoc();
	
	// 자식 프로세스 생성
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (0);
	}
	
	if (pid == 0)
	{
		// 시그널 핸들러 설정
		signal(SIGINT, heredoc_signal_handler);
		
		// 터미널 설정을 변경하여 입력이 화면에 표시되도록 함
		struct termios term;
		if (tcgetattr(STDIN_FILENO, &term) == 0)
		{
			// 입력 에코 활성화, 컨트롤 문자 표시 비활성화
			term.c_lflag |= ECHO;
			term.c_lflag &= ~ECHOCTL;
			tcsetattr(STDIN_FILENO, TCSANOW, &term);
		}
		
		// 히어독 처리
		process_heredoc_lines(shell, delimiter, fd);
		
		// 자식 프로세스 종료
		exit(0);
	}
	
	// 부모 프로세스에서 자식 프로세스 대기
	waitpid(pid, &status, 0);
	setup_signals_interactive();
	close(fd);
	
	// 시그널에 의한 종료 처리
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		g_signal = SIGINT;
		shell->status.exit_code = 130;
		return (0);
	}
	
	// 비정상 종료 처리
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		if (WEXITSTATUS(status) == 130)
		{
			g_signal = SIGINT;
			shell->status.exit_code = 130;
		}
		return (0);
	}
	
	// 전역 시그널 상태 복원
	g_signal = original_g_signal;
	return (WIFEXITED(status) && WEXITSTATUS(status) == 0);
}

int	setup_heredoc_read(t_shell *shell, t_heredoc_entry *entry, char *temp_file)
{
	int	fd;

	fd = open(temp_file, O_RDONLY);
	if (fd == -1)
	{
		unlink(temp_file);
		shell_free(shell, temp_file);
		return (1);
	}
	entry->fd = fd;
	shell->heredoc.temp_file = temp_file;
	entry->next = shell->heredoc.entries;
	shell->heredoc.entries = entry;
	shell->heredoc.count++;
	return (0);
}

int	handle_heredoc(t_shell *shell, char *delimiter)
{
	char			*temp_file;
	int				fd;
	t_heredoc_entry	*entry;
	int				success;

	if (setup_heredoc_file(shell, &temp_file, &fd))
		return (1);
	entry = setup_heredoc_entry(shell, delimiter, temp_file, &fd);
	if (!entry)
		return (1);
	success = read_heredoc_content(shell, delimiter, fd);
	if (!success)
	{
		unlink(temp_file);
		shell_free(shell, temp_file);
		return (1);
	}
	return (setup_heredoc_read(shell, entry, temp_file));
}

int	get_heredoc_fd(t_shell *shell, char *delimiter)
{
	t_heredoc_entry	*current;

	current = shell->heredoc.entries;
	while (current)
	{
		if (ft_strcmp(current->delimiter, delimiter) == 0)
			return (current->fd);
		current = current->next;
	}
	return (-1);
}
