/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 05:47:23 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 05:52:52 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_setup(t_shell *shell)
{
	if (g_signal == 130 || g_signal == 131)
	{
		shell->status.exit_code = g_signal;
		g_signal = 0;
	}
}

void	exit_loop(t_shell *shell)
{
	free(shell->input_line);
	shell->input_line = NULL;
	exit_setup(shell);
}
