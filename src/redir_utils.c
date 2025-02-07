/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:56:34 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/04 05:10:31 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 입력 리다이렉션 처리
static int setup_input_redirect(t_redirection *redir)
{
    int fd;

    fd = open(redir->filename, O_RDONLY);
    if (fd == -1)
    {
        printf("minishell: %s: No such file or directory\n", redir->filename);
        return (1);
    }
    
    // 표준 입력을 파일로 변경
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        close(fd);
        return (1);
    }
    close(fd);
    return (0);
}

// 출력 리다이렉션 처리
static int setup_output_redirect(t_redirection *redir)
{
    int fd;
    int flags;

    // 일반 출력(>)인지 추가(>>)인지에 따라 플래그 설정
    if (redir->type == REDIR_OUT)
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    else  // REDIR_APPEND
        flags = O_WRONLY | O_CREAT | O_APPEND;

    fd = open(redir->filename, flags, 0644);
    if (fd == -1)
    {
        printf("minishell: %s: Permission denied\n", redir->filename);
        return (1);
    }

    // 표준 출력을 파일로 변경
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        close(fd);
        return (1);
    }
    close(fd);
    return (0);
}

// heredoc 처리
static int setup_heredoc(t_shell *shell, t_redirection *redir)
{
    int pipefd[2];
    char *line;

    (void)shell;
    if (pipe(pipefd) == -1)
        return (1);

    while (1)
    {
        line = readline("> ");
        if (!line || strcmp(line, redir->filename) == 0)
        {
            free(line);
            break;
        }
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }

    close(pipefd[1]);
    if (dup2(pipefd[0], STDIN_FILENO) == -1)
    {
        close(pipefd[0]);
        return (1);
    }
    close(pipefd[0]);
    return (0);
}

// 모든 리다이렉션 설정
int setup_redirections(t_shell *shell, t_redirection *redirs)
{
    t_redirection *current;

    current = redirs;
    while (current)
    {
        switch (current->type)
        {
            case REDIR_IN:
                if (setup_input_redirect(current) != 0)
                    return (1);
                break;
            case REDIR_OUT:
            case REDIR_APPEND:
                if (setup_output_redirect(current) != 0)
                    return (1);
                break;
            case REDIR_HEREDOC:
                if (setup_heredoc(shell, current) != 0)
                    return (1);
                break;
        }
        current = current->next;
    }
    return (0);
}