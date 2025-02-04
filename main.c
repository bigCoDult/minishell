/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:21:05 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/04 05:16:25 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal;

static void	init_shell(t_shell *shell, char **envp)
{
	printf("DEBUG: Initializing shell\n");
	shell->memory = NULL;
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->status.exit_code = 0;
	shell->env = init_env(shell, envp);
	printf("DEBUG: Shell initialization completed\n");
}

static void	minishell_loop(t_shell *shell)
{
	printf("DEBUG: Starting minishell loop\n");
	
	while (1)
	{
		printf("DEBUG: Waiting for command input\n");
		shell->input_line = readline("MINISHELL$> ");
		
		if (!shell->input_line)
		{
			printf("DEBUG: EOF detected (Ctrl+D)\n");
			break;
		}
		
		if (shell->input_line[0] != '\0')
		{
			printf("DEBUG: Processing command: %s\n", shell->input_line);
			add_history(shell->input_line);
			
			if (parse_input(shell) == 0)
			{
				printf("DEBUG: Command parsed successfully\n");
				execute_commands(shell);
			}
			else
			{
				printf("DEBUG: Command parsing failed\n");
			}
		}
		else
		{
			printf("DEBUG: Empty command\n");
		}
		
		free(shell->input_line);
		shell->input_line = NULL;
		printf("DEBUG: Command processing completed\n");
	}
	printf("DEBUG: Exiting minishell loop\n");
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	printf("DEBUG: minishell starting\n");
	
	(void)argv;
	if (argc != 1)
	{
		printf("DEBUG: Invalid argument count\n");
		printf("Just a Arg PLZ.\n");
		return (1);
	}
	
	printf("DEBUG: Setting up signal handlers\n");
	g_signal = 0;
	
	printf("DEBUG: Initializing shell environment\n");
	init_shell(&shell, envp);
	
	printf("DEBUG: Setting up signal handlers\n");
	setup_signals();
	
	printf("DEBUG: Starting main shell loop\n");
	minishell_loop(&shell);
	
	printf("DEBUG: Cleaning up memory\n");
	free_all_memory(&shell);
	
	printf("DEBUG: minishell exiting with status: %d\n", shell.status.exit_code);
	return (shell.status.exit_code);
}
