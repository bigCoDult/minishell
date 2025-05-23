/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:21:05 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 05:51:00 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal;

void	setup_terminal_settings(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	restore_terminal_settings(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

static void	init_shell(t_shell *shell, char **envp)
{
	shell->memory = NULL;
	shell->tokens = NULL;
	shell->status.exit_code = 0;
	shell->env = init_env(shell, envp);
	shell->heredoc.original_stdin = -1;
	shell->heredoc.entries = NULL;
	shell->heredoc.count = 0;
	shell->original_stdout = -1;
	shell->original_stderr = -1;
}

static void	minishell_loop(t_shell *shell)
{
	while (1)
	{
		exit_setup(shell);
		if (shell->heredoc.original_stdin != -1 || shell->original_stdout != -1)
			restore_io(shell);
		exit_setup(shell);
		shell->input_line = readline("MINISHELL$> ");
		exit_setup(shell);
		if (!shell->input_line)
		{
			exit_setup(shell);
			break ;
		}
		if (shell->input_line[0] != '\0')
		{
			add_history(shell->input_line);
			if (parse_input(shell) == 0)
			{
				execute_commands(shell);
				free_command_memory(shell);
			}
		}
		exit_loop(shell);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	ft_memset(&shell, 0, sizeof(t_shell));
	(void)argv;
	if (argc != 1)
	{
		printf("Just a Arg PLZ.\n");
		return (1);
	}
	g_signal = 0;
	init_shell(&shell, envp);
	setup_terminal_settings();
	setup_signals();
	minishell_loop(&shell);
	restore_terminal_settings();
	free_all_memory(&shell);
	free_env(&shell);
	return (shell.status.exit_code);
}
