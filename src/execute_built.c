/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:42:51 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/25 11:42:12 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	n_option;

	n_option = 0;
	i = 1;
	if (args[1] && strcmp(args[1], "-n") == 0)
	{
		n_option = 1;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!n_option)
		printf("\n");
	return (0);
}

int	builtin_cd(t_shell *shell, char **args)
{
	char	*path;
	char	current_path[1024];

	if (!args[1])
		path = getenv("HOME");
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		printf("cd: %s: No such file or directory\n", path);
		return (1);
	}
	if (getcwd(current_path, sizeof(current_path)))
		set_env_value(shell, "PWD", current_path);
	return (0);
}

int	builtin_pwd(void)
{
	char	current_path[1024];

	if (getcwd(current_path, sizeof(current_path)))
	{
		printf("%s\n", current_path);
		return (0);
	}
	return (1);
}

int	builtin_export(t_shell *shell, char **args)
{
	char	*key;
	char	*value;
	int		i;

	i = 1;
	while (args[i])
	{
		debug_print(2047, 2, "DEBUG: [builtin_export] Processing arg: %s\n", args[i]);
		if (parse_env_arg(shell, args[i], &key, &value))
		{
			printf("export: '%s': not a valid identifier\n", args[i]);
			return (1);
		}
		set_env_value(shell, key, value);
		shell_free(shell, key);
		if (value)
			shell_free(shell, value);
		i++;
	}
	return (0);
}

int	builtin_unset(t_shell *shell, char **args)
{
	int		i;
	t_env	*current;
	t_env	*prev;

	i = 1;
	while (args[i])
	{
		current = shell->env;
		prev = NULL;
		while (current)
		{
			if (strcmp(current->key, args[i]) == 0)
			{
				if (prev)
					prev->next = current->next;
				else
					shell->env = current->next;
				shell_free(shell, current->key);
				shell_free(shell, current->value);
				shell_free(shell, current);
				break;
			}
			prev = current;
			current = current->next;
		}
		i++;
	}
	return (0);
}

int	builtin_env(t_shell *shell)
{
	t_env	*current;

	debug_print(2047, 2, "DEBUG: [builtin_env] Starting to print environment\n");
	current = shell->env;
	while (current)
	{
		if (current->value)
		{
			debug_print(2047, 2, "DEBUG: [builtin_env] Printing: %s=%s\n",
				   current->key, current->value);
			printf("%s=%s\n", current->key, current->value);
		}
		current = current->next;
	}
	return (0);
}

int	builtin_exit(t_shell *shell, char **args)
{
	int	exit_code;

	if (args[1])
		exit_code = atoi(args[1]);
	else
		exit_code = shell->status.exit_code;
	printf("exit\n");
	free_all_memory(shell);
	free_env(shell);
	exit(exit_code);
}
