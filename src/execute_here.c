/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_here.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:13:01 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/14 15:48:21 by yutsong          ###   ########.fr       */
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

    // 현재 표준 입력 저장
    shell->heredoc.original_stdin = dup(STDIN_FILENO);
    if (shell->heredoc.original_stdin == -1)
        return 1;

    // 임시 파일 생성
    shell->heredoc.temp_file = create_temp_heredoc_file(shell);
    if (!shell->heredoc.temp_file)
        return 1;

    // 임시 파일 열기
    shell->heredoc.fd = open(shell->heredoc.temp_file, 
                           O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (shell->heredoc.fd == -1)
        return 1;

    pid = fork();
    if (pid == 0)
    {
        signal(SIGINT, heredoc_signal_handler);
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
            write(shell->heredoc.fd, line, strlen(line));
            write(shell->heredoc.fd, "\n", 1);
            free(line);
        }
    }

    waitpid(pid, &status, 0);
    
    // 표준 입력 복원
    if (shell->heredoc.original_stdin != -1)
    {
        dup2(shell->heredoc.original_stdin, STDIN_FILENO);
        close(shell->heredoc.original_stdin);
    }

    close(shell->heredoc.fd);

    if (WIFSIGNALED(status) || (WIFEXITED(status) && WEXITSTATUS(status) != 0))
    {
        unlink(shell->heredoc.temp_file);
        return 1;
    }

    // 읽기 모드로 다시 열기
    shell->heredoc.fd = open(shell->heredoc.temp_file, O_RDONLY);
    if (shell->heredoc.fd == -1)
    {
        unlink(shell->heredoc.temp_file);
        return 1;
    }

    return 0;
}

void cleanup_heredoc(t_shell *shell)
{
    if (shell->heredoc.fd != -1)
    {
        close(shell->heredoc.fd);
        shell->heredoc.fd = -1;
    }
    if (shell->heredoc.temp_file)
    {
        unlink(shell->heredoc.temp_file);
        shell_free(shell, shell->heredoc.temp_file);
        shell->heredoc.temp_file = NULL;
    }
}