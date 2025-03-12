/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 04:42:51 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/12 14:20:13 by yutsong          ###   ########.fr       */
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
	
	printf("exit\n");
	
	if (args[1])
	{
		// 인자가 숫자인지 확인
		i = 0;
		if (args[1][i] == '-' || args[1][i] == '+')
			i++;
		
		while (args[1][i])
		{
			if (!ft_isdigit(args[1][i]))
			{
				fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
				free_all_memory(shell);
				free_env(shell);
				exit(255);
			}
			i++;
		}
		
		// 여러 인자가 주어진 경우
		if (args[2])
		{
			fprintf(stderr, "minishell: exit: too many arguments\n");
			return (1);  // 종료하지 않고 에러 코드 반환
		}
		
		exit_code = ft_atoi(args[1]);
	}
	else
		exit_code = shell->status.exit_code;
	
	free_all_memory(shell);
	free_env(shell);
	exit(exit_code);
}
