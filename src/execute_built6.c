/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built6.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 08:11:51 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 08:16:11 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_builtin_exit(t_shell *shell, int exit_code)
{
	free_all_memory(shell);
	free_env(shell);
	exit(exit_code);
}

static int	check_arg_length(t_shell *shell, char *arg)
{
	size_t	len;

	len = ft_strlen(arg);
	if (len > 20 || (len == 20 && arg[0] != '-'
			&& ft_strcmp(arg, "9223372036854775807") > 0)
		|| (len == 20 && arg[0] == '-'
			&& ft_strcmp(arg, "-9223372036854775808") > 0))
	{
		print_error("minishell: exit: %s: numeric argument required\n", arg);
		handle_builtin_exit(shell, 2);
		return (1);
	}
	return (0);
}

static int	check_numeric_arg(t_shell *shell, char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			print_error(
				"minishell: exit: %s: numeric argument required\n", arg);
			handle_builtin_exit(shell, 2);
			return (1);
		}
		i++;
	}
	return (0);
}

int	builtin_exit(t_shell *shell, char **args)
{
	int	exit_code;

	printf("exit\n");
	if (args[1])
	{
		if (check_arg_length(shell, args[1])
			|| check_numeric_arg(shell, args[1]))
			return (1);
		if (args[2])
		{
			print_error("minishell: exit: too many arguments\n");
			return (1);
		}
		exit_code = ft_atoi(args[1]);
	}
	else
		exit_code = shell->status.exit_code;
	handle_builtin_exit(shell, exit_code);
	return (0);
}
