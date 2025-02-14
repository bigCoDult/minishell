/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 03:59:33 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/14 14:37:01 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void signal_handler(int signo)
{
    // SIGINT (Ctrl+C) 처리
    if (signo == SIGINT)
    {
        g_signal = signo;
        write(STDOUT_FILENO, "\n", 1);  // 새로운 프롬프트 라인으로 이동
        rl_on_new_line();               // readline에게 새 라인에 있다고 알림
        rl_replace_line("", 0);         // 현재 입력 라인을 지움
        rl_redisplay();                 // 프롬프트 다시 표시
    }
    // SIGQUIT (Ctrl+\) 처리
    else if (signo == SIGQUIT)
    {
        g_signal = signo;
        rl_on_new_line();
        rl_redisplay();
    }
}

void heredoc_signal_handler(int signo)
{
    if (signo == SIGINT)
    {
        g_signal = 1;
        write(1, "\n", 1);
        close(STDIN_FILENO);
    }
}

void setup_signals(void)
{
    // SIGINT (Ctrl+C) 핸들러 설정
    signal(SIGINT, signal_handler);
    
    // SIGQUIT (Ctrl+\) 핸들러 설정
    signal(SIGQUIT, signal_handler);
    
    // SIGTERM 무시
    signal(SIGTERM, SIG_IGN);
}