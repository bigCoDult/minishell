/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 03:59:33 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 17:48:02 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		g_signal = 0;
		rl_on_new_line();
		rl_redisplay();
	}
}

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

void	heredoc_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signal = signo;
		write(STDOUT_FILENO, "\n", 1);
		close(STDIN_FILENO);
	}
}

void	setup_signals_interactive(void)
{
	signal(SIGINT, interactive_signal_handler);
	signal(SIGQUIT, interactive_signal_handler);
	signal(SIGTERM, SIG_IGN);
}

void	setup_signals_executing(void)
{
	signal(SIGINT, executing_signal_handler);
	signal(SIGQUIT, executing_signal_handler);
	signal(SIGTERM, SIG_IGN);
}

void	setup_signals_heredoc(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}

void	setup_signals(void)
{
	setup_signals_interactive();
}
