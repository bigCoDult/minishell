/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:21:05 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/04 00:18:53 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal;

static void	init_shell(t_shell *shell, char **envp)
{
	(void)envp;
	// shell->env = init_env(envp);
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->memory = NULL;
	shell->status.exit_code = 0;
}

static void	minishell_loop(t_shell *shell)
{
	while (1)
	{
		shell->input_line = readline("MINISHELL$> ");
		if (!shell->input_line)
			break ;
		if (shell->input_line[0] != '\0')
		{
			add_history(shell->input_line);
			// if (parse_input(shell) == 0)
			// 	execute_commands(shell);
		}
		free(shell->input_line);
		shell->input_line = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argv;
	if (argc != 1)
	{
		printf("Just a Arg PLZ.\n");
		return (1);
	}
	g_signal = 0;
	init_shell(&shell, envp);
	// setup_signals();
	minishell_loop(&shell);
	// free_all_memory(&shell);
	return (shell.status.exit_code);
}
