/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:13:01 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/14 14:49:36 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 히어독 처리 함수
int handle_heredoc(t_shell *shell, char *delimiter)
{
    int pipe_fd[2];
    int status;
    pid_t pid;
    char *line;

    if (pipe(pipe_fd) == -1)
        return 1;

    pid = fork();
    if (pid == 0)
    {
        signal(SIGINT, heredoc_signal_handler);
        close(pipe_fd[0]);
        while (1)
        {
            line = readline("> ");
            if (!line || g_signal || strcmp(line, delimiter) == 0)
            {
                free(line);
                if (g_signal)
                    exit(1);
                exit(0);
            }
            write(pipe_fd[1], line, strlen(line));
            write(pipe_fd[1], "\n", 1);
            free(line);
        }
    }

    close(pipe_fd[1]);
    waitpid(pid, &status, 0);

    if (WIFSIGNALED(status) || (WIFEXITED(status) && WEXITSTATUS(status) != 0))
    {
        close(pipe_fd[0]);
        return 1;
    }

    // 파이프의 읽기 끝을 저장
    shell->heredoc.pipe_fd[0] = pipe_fd[0];
    return 0;
}