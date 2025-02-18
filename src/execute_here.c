/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:13:01 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/18 19:09:05 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *create_temp_heredoc_file(t_shell *shell)
{
    static int count = 0;
    char *filename;
    char num[32];

    sprintf(num, "%d", count++);
    filename = shell_malloc(shell, ft_strlen("/tmp/heredoc_") + ft_strlen(num) + 1);
    if (!filename)
        return NULL;
    strcpy(filename, "/tmp/heredoc_");
    strcat(filename, num);
    return filename;
}

int handle_heredoc(t_shell *shell, char *delimiter)
{
    pid_t pid;
    int status;
    int pipe_fds[2];

    if (pipe(pipe_fds) == -1)
        return (1);

    pid = fork();
    if (pid == 0)
    {
        close(pipe_fds[0]);
        while (1)
        {
            char *line = readline("> ");
            if (!line || strcmp(line, delimiter) == 0)
            {
                free(line);
                close(pipe_fds[1]);
                // exit(0);
				free_exit(shell, 0);
            }
            write(pipe_fds[1], line, strlen(line));
            write(pipe_fds[1], "\n", 1);
            free(line);
        }
    }

    close(pipe_fds[1]);
    waitpid(pid, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
    {
        shell->heredoc.fd = pipe_fds[0];
        return (0);
    }

    close(pipe_fds[0]);
    return (1);
}

void cleanup_heredoc(t_shell *shell)
{
    // readline 상태 초기화
    rl_on_new_line();
    rl_replace_line("", 0);

    if (shell->heredoc.fd != -1)
    {
        close(shell->heredoc.fd);
        shell->heredoc.fd = -1;
    }
    if (shell->heredoc.original_stdin != -1)
    {
        dup2(shell->heredoc.original_stdin, STDIN_FILENO);
        close(shell->heredoc.original_stdin);
        shell->heredoc.original_stdin = -1;
    }
    if (shell->heredoc.temp_file)
    {
        unlink(shell->heredoc.temp_file);
        shell_free(shell, shell->heredoc.temp_file);
        shell->heredoc.temp_file = NULL;
    }
}