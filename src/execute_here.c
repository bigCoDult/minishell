/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:13:01 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/21 05:22:46 by yutsong          ###   ########.fr       */
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

t_heredoc_entry *create_heredoc_entry(t_shell *shell, char *delimiter)
{
    t_heredoc_entry *entry;

    entry = shell_malloc(shell, sizeof(t_heredoc_entry));
    if (!entry)
        return (NULL);

    entry->delimiter = shell_strdup(shell, delimiter);
    if (!entry->delimiter)
    {
        shell_free(shell, entry);
        return (NULL);
    }

    entry->fd = -1;
    entry->next = NULL;
    return (entry);
}

int handle_heredoc(t_shell *shell, char *delimiter)
{
    pid_t pid;
    int status;
    int pipe_fds[2];
    t_heredoc_entry *entry;

    entry = create_heredoc_entry(shell, delimiter);
    if (!entry)
        return (1);

    entry->next = shell->heredoc.entries;
    shell->heredoc.entries = entry;
    shell->heredoc.count++;

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
        entry->fd = pipe_fds[0];
        return (0);
    }

    close(pipe_fds[0]);
    return (1);
}

int get_heredoc_fd(t_shell *shell, char *delimiter)
{
    t_heredoc_entry *current = shell->heredoc.entries;

    while (current)
    {
        if (strcmp(current->delimiter, delimiter) == 0)
            return (current->fd);
        current = current->next;
    }
    return (-1);
}

void cleanup_heredoc(t_shell *shell)
{
    t_heredoc_entry *current = shell->heredoc.entries;
    t_heredoc_entry *next;

    while (current)
    {
        next = current->next;
        if (current->fd != -1)
        {
            close(current->fd);
            current->fd = -1;
        }
        shell_free(shell, current->delimiter);
        shell_free(shell, current);
        current = next;
    }
    shell->heredoc.entries = NULL;
    shell->heredoc.count = 0;

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

// 명령어에 해당하는 히어독 FD 찾기
int find_command_heredoc_fd(t_shell *shell, t_command *cmd)
{
    t_redirection *redir = cmd->redirs;
    while (redir)
    {
        if (redir->type == REDIR_HEREDOC)
        {
            t_heredoc_entry *entry = shell->heredoc.entries;
            while (entry)
            {
                if (strcmp(entry->delimiter, redir->filename) == 0)
                    return entry->fd;
                entry = entry->next;
            }
        }
        redir = redir->next;
    }
    return -1;
}

// 명령어 실행 전 히어독 설정
void setup_command_heredoc(t_shell *shell, t_command *cmd)
{
    int heredoc_fd = find_command_heredoc_fd(shell, cmd);
    if (heredoc_fd != -1)
    {
        dup2(heredoc_fd, STDIN_FILENO);
        close(heredoc_fd);
    }
}