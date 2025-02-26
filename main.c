/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:21:05 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/25 11:42:12 by sanbaek          ###   ########.fr       */
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
	shell->heredoc.entries = NULL;
	shell->heredoc.count = 0;
	shell->heredoc.temp_file = NULL;
	debug_print(2047, 7, "DEBUG: Shell initialization completed\n");
}

static void	minishell_loop(t_shell *shell)
{
	debug_print(2047, 7, "DEBUG: Starting minishell loop\n");
	
	while (1)
	{
		g_signal = 0;  // 새로운 명령어 입력 전 g_signal 리셋
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
	// free_all_memory()가 환경변수를 free하지 못하고 있어서 임시로 추가
	free_env(&shell);
	
	debug_print(2047, 7, "DEBUG: minishell exiting with status: %d\n", shell.status.exit_code);
	return (shell.status.exit_code);
}
