/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:31:16 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/04 04:32:30 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void setup_pipe(t_shell *shell, int pipefd[2], int is_first, int is_last)
{
    if (!is_first)  // 첫 번째 명령어가 아니면
    {
        dup2(shell->pipe.read_fd, STDIN_FILENO);  // 이전 파이프의 읽기를 표준 입력으로
        close(shell->pipe.read_fd);
    }
    
    if (!is_last)  // 마지막 명령어가 아니면
    {
        dup2(pipefd[1], STDOUT_FILENO);  // 현재 파이프의 쓰기를 표준 출력으로
    }
    
    close(pipefd[0]);
    close(pipefd[1]);
}

int execute_piped_command(t_shell *shell, t_command *cmd, int is_first, int is_last)
{
    pid_t pid;
    int pipefd[2];
    
    if (!is_last && pipe(pipefd) < 0)
        return (1);
    
    pid = fork();
    if (pid == 0)
    {
        // 자식 프로세스
        setup_pipe(shell, pipefd, is_first, is_last);
        execvp(cmd->name, cmd->args);
        exit(127);  // 명령어를 찾을 수 없음
    }
    else if (pid < 0)
    {
        return (1);
    }
    
    // 부모 프로세스
    if (!is_first)
        close(shell->pipe.read_fd);
    if (!is_last)
    {
        close(pipefd[1]);
        shell->pipe.read_fd = pipefd[0];  // 다음 명령어를 위해 읽기 fd 저장
    }
    
    return (0);
}
