/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:13:01 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/17 20:17:14 by sanbaek          ###   ########.fr       */
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
    char *line;

    g_signal = 0;
    void (*old_int_handler)(int) = signal(SIGINT, SIG_IGN);
    void (*old_quit_handler)(int) = signal(SIGQUIT, SIG_IGN);

    shell->heredoc.original_stdin = dup(STDIN_FILENO);
    if (shell->heredoc.original_stdin == -1)
    {
        signal(SIGINT, old_int_handler);
        signal(SIGQUIT, old_quit_handler);
        return 1;
    }

    shell->heredoc.temp_file = create_temp_heredoc_file(shell);
    if (!shell->heredoc.temp_file)
    {
        close(shell->heredoc.original_stdin);
        return 1;
    }

    shell->heredoc.fd = open(shell->heredoc.temp_file, 
                           O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (shell->heredoc.fd == -1)
    {
        cleanup_heredoc(shell);
        return 1;
    }

    pid = fork();
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGINT, heredoc_signal_handler);
        while (1)
        {
            line = readline("> ");
            if (!line || g_signal || strcmp(line, delimiter) == 0)
            {
                free(line);
                if (g_signal)
                {
                    close(shell->heredoc.fd);
					exit_with_free(shell, 1);
                    // exit(1);
                }
				exit_with_free(shell, 0);
                // exit(0);
            }
            write(shell->heredoc.fd, line, strlen(line));
            write(shell->heredoc.fd, "\n", 1);
            free(line);
        }
    }

    waitpid(pid, &status, 0);
    
    signal(SIGINT, old_int_handler);
    signal(SIGQUIT, old_quit_handler);

    if (WIFSIGNALED(status))
    {
        g_signal = 1;
        cleanup_heredoc(shell);
        return 1;
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
    {
        cleanup_heredoc(shell);
        return 1;
    }

    shell->heredoc.fd = open(shell->heredoc.temp_file, O_RDONLY);
    if (shell->heredoc.fd == -1)
    {
        cleanup_heredoc(shell);
        return 1;
    }

    return 0;
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