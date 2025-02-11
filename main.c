/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:21:05 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/11 18:57:24 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal;

static void	init_shell(t_shell *shell, char **envp)
{
	debug_print(2047, 7, "DEBUG: Initializing shell\n");
	shell->memory = NULL;
	shell->tokens = NULL;
	shell->status.exit_code = 0;
	shell->env = init_env(shell, envp);
	shell->heredoc.original_stdin = -1;
	shell->heredoc.delimiter = NULL;
	shell->heredoc.pipe_fd[0] = -1;
	shell->heredoc.pipe_fd[1] = -1;
	debug_print(2047, 7, "DEBUG: Shell initialization completed\n");
}

static void	minishell_loop(t_shell *shell)
{
	debug_print(2047, 7, "DEBUG: Starting minishell loop\n");
	
	while (1)
	{
		debug_print(2047, 7, "DEBUG: Waiting for command input\n");
		shell->input_line = readline("MINISHELL$> ");
		
		if (!shell->input_line)
		{
			debug_print(2047, 7, "DEBUG: EOF detected (Ctrl+D)\n");
			break;
		}
		
		if (shell->input_line[0] != '\0')
		{
			debug_print(2047, 7, "DEBUG: Processing command: %s\n", shell->input_line);
			add_history(shell->input_line);
			
			if (parse_input(shell) == 0)
			{
				debug_print(2047, 7, "DEBUG: Command parsed successfully\n");
				execute_commands(shell);
			}
			else
			{
				debug_print(2047, 7, "DEBUG: Command parsing failed\n");
			}
		}
		else
		{
			debug_print(2047, 7, "DEBUG: Empty command\n");
		}
		
		free(shell->input_line);
		shell->input_line = NULL;
		debug_print(2047, 7, "DEBUG: Command processing completed\n");
	}
	debug_print(2047, 7, "DEBUG: Exiting minishell loop\n");
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	
	ft_memset(&shell, 0, sizeof(t_shell));
	debug_print(2047, 7, "DEBUG: minishell starting\n");
	
	(void)argv;
	if (argc != 1)
	{
		debug_print(2047, 7, "DEBUG: Invalid argument count\n");
		printf("Just a Arg PLZ.\n");
		return (1);
	}
	
	debug_print(2047, 7, "DEBUG: Setting up signal handlers\n");
	g_signal = 0;
	
	debug_print(2047, 7, "DEBUG: Initializing shell environment\n");
	init_shell(&shell, envp);
	
	debug_print(2047, 7, "DEBUG: Setting up signal handlers\n");
	setup_signals();
	
	debug_print(2047, 7, "DEBUG: Starting main shell loop\n");
	minishell_loop(&shell);
	
	debug_print(2047, 7, "DEBUG: Cleaning up memory\n");
	free_all_memory(&shell);
	
	debug_print(2047, 7, "DEBUG: minishell exiting with status: %d\n", shell.status.exit_code);
	return (shell.status.exit_code);
}
