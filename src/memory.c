/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:24:40 by yutsong           #+#    #+#             */
/*   Updated: 2025/02/25 11:42:12 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*shell_malloc(t_shell *shell, size_t size)
{
	void		*ptr;
	t_memory	*new_node;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	new_node = malloc(sizeof(t_memory));
	if (!new_node)
	{
		free(ptr);
		return (NULL);
	}
	new_node->addr = ptr;
	new_node->next = shell->memory;
	shell->memory = new_node;
	return (ptr);
}

void	shell_free(t_shell *shell, void *ptr)
{
	t_memory	*current;
	t_memory	*prev;

	if (!ptr)
		return ;
	prev = NULL;
	current = shell->memory;
	while (current)
	{
		if (current->addr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				shell->memory = current->next;
			free(ptr);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	free_command_memory(t_shell *shell)
{
	t_memory	*current;
	t_memory	*next;
	t_memory	*prev;
	t_env		*env_ptr;

	env_ptr = shell->env;
	current = shell->memory;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (current->addr != env_ptr
			&& (current->addr < (void *)env_ptr
				|| current->addr > (void *)(env_ptr + sizeof(t_env))))
		{
			free(current->addr);
			free(current);
			if (prev)
				prev->next = next;
			else
				shell->memory = next;
		}
		else
			prev = current;
		current = next;
	}
}

void	free_all_memory(t_shell *shell)
{
	t_memory	*current;
	t_memory	*next;
	t_memory	*prev;
	int			is_env;
	t_env		*env_ptr;

	debug_print(2047, 11, "DEBUG: [free_all_memory] Starting to free all memory\n");
	current = shell->memory;
	prev = NULL;
	while (current)
	{
		next = current->next;
		env_ptr = shell->env;
		is_env = 0;
		while (env_ptr)
		{
			if (current->addr == env_ptr
				|| current->addr == env_ptr->key
				|| current->addr == env_ptr->value)
			{
				is_env = 1;
				break ;
			}
			env_ptr = env_ptr->next;
		}
		if (!is_env)
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

static void	free_shell_malloc(t_shell *shell)
{
	t_memory	*current;
	t_memory	*next;

	debug_print(2047, 11, "DEBUG: [free_all_memory] Starting to free all memory\n");
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
