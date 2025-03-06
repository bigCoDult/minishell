/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:24:40 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/05 13:44:52 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_env_memory(t_shell *shell, void *addr)
{
	t_env	*env_ptr;

	env_ptr = shell->env;
	while (env_ptr)
	{
		if (addr == env_ptr
			|| addr == env_ptr->key
			|| addr == env_ptr->value)
			return (1);
		env_ptr = env_ptr->next;
	}
	return (0);
}

void	free_all_memory(t_shell *shell)
{
	t_memory	*current;
	t_memory	*next;
	t_memory	*prev;

	current = shell->memory;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (!is_env_memory(shell, current->addr))
		{
			if (prev)
				prev->next = next;
			else
				shell->memory = next;
			free(current->addr);
			free(current);
		}
		else
			prev = current;
		current = next;
	}
}

void	free_shell_malloc(t_shell *shell)
{
	t_memory	*current;
	t_memory	*next;

	current = shell->memory;
	while (current)
	{
		next = current->next;
		free(current->addr);
		free(current);
		current = next;
	}
}

void	free_env(t_shell *shell)
{
	t_env	*current;
	t_env	*next;

	current = shell->env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

void	free_exit(t_shell *shell, int status)
{
	free_shell_malloc(shell);
	free_env(shell);
	exit(status);
}
