/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:09:47 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/19 11:00:02 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	interactive_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		g_signal = 130;
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

void	executing_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signal = 130;
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (signo == SIGQUIT)
	{
		g_signal = 131;
		write(STDOUT_FILENO, "Quit\n", 5);
	}
}

void	heredoc_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signal = 130;
		write(STDOUT_FILENO, "\n", 1);
		close(STDIN_FILENO);
	}
}
