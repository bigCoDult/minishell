/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 03:59:33 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/06 19:10:41 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
