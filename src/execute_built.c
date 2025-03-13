/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:42:51 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 07:51:30 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_env_var(t_shell *shell, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = shell->env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				shell->env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			break ;
		}
		prev = current;
		current = current->next;
	}
}

int	builtin_unset(t_shell *shell, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		remove_env_var(shell, args[i]);
		i++;
	}
	return (0);
}

int	builtin_env(t_shell *shell)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}

int	builtin_exit(t_shell *shell, char **args)
{
	int		exit_code;
	int		i;
	char	*arg;
	size_t	len;

	printf("exit\n");
	if (args[1])
	{
		arg = args[1];
		len = ft_strlen(arg);
		if (len > 20 || (len == 20 && arg[0] != '-'
				&& ft_strcmp(arg, "9223372036854775807") > 0)
			|| (len == 20 && arg[0] == '-'
				&& ft_strcmp(arg, "-9223372036854775808") > 0))
		{
			print_error(
				"minishell: exit: %s: numeric argument required\n", arg);
			free_all_memory(shell);
			free_env(shell);
			exit(2);
		}
		i = 0;
		if (arg[i] == '-' || arg[i] == '+')
			i++;
		while (arg[i])
		{
			if (!ft_isdigit(arg[i]))
			{
				print_error(
					"minishell: exit: %s: numeric argument required\n", arg);
				free_all_memory(shell);
				free_env(shell);
				exit(2);
			}
			i++;
		}
		if (args[2])
		{
			print_error("minishell: exit: too many arguments\n");
			return (1);
		}
		exit_code = ft_atoi(arg);
	}
	else
		exit_code = shell->status.exit_code;
	free_all_memory(shell);
	free_env(shell);
	exit(exit_code);
}
