/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 03:59:33 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 16:50:33 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 대화형 모드에서의 시그널 핸들러 (명령어 입력 중)
static void	interactive_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signal = signo;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signo == SIGQUIT)
	{
		g_signal = 0;  // SIGQUIT은 무시
		rl_on_new_line();
		rl_redisplay();
	}
}

// 명령어 실행 중 시그널 핸들러
static void	executing_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signal = signo;
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (signo == SIGQUIT)
	{
		g_signal = signo;
		write(STDOUT_FILENO, "Quit\n", 5);
	}
}

// 히어독 모드에서의 시그널 핸들러
void	heredoc_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signal = signo;
		write(STDOUT_FILENO, "\n", 1);
		close(STDIN_FILENO);  // readline 종료를 위한 stdin 닫기
	}
}

// 대화형 모드 시그널 설정 (명령어 입력 중)
void	setup_signals_interactive(void)
{
	signal(SIGINT, interactive_signal_handler);
	signal(SIGQUIT, interactive_signal_handler);
	signal(SIGTERM, SIG_IGN);
}

// 명령어 실행 중 시그널 설정
void	setup_signals_executing(void)
{
	signal(SIGINT, executing_signal_handler);
	signal(SIGQUIT, executing_signal_handler);
	signal(SIGTERM, SIG_IGN);
}

// 히어독 모드 시그널 설정
void	setup_signals_heredoc(void)
{
	signal(SIGINT, SIG_IGN);  // 부모 프로세스에서는 무시
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}

// 기존 호환성 유지를 위한 함수
void	setup_signals(void)
{
	setup_signals_interactive();
}
