/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:37:51 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 04:34:43 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (current->addr != env_ptr && (current->addr < (void *)env_ptr
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

static int	is_heredoc_memory(t_shell *shell, void *addr)
{
	t_heredoc_entry	*entry;

	if (!shell->heredoc.entries)
		return (0);
	entry = shell->heredoc.entries;
	if (addr == entry
		|| (addr >= (void *)entry
			&& addr < (void *)entry + sizeof(t_heredoc_entry)
			* shell->heredoc.count))
		return (1);
	return (0);
}

static int	is_a_env_memory(t_shell *shell, void *addr)
{
	t_env	*env_ptr;

	env_ptr = shell->env;
	if (addr == env_ptr
		|| (addr >= (void *)env_ptr
			&& addr < (void *)(env_ptr + sizeof(t_env))))
		return (1);
	return (0);
}

static void	free_memory_node(t_shell *shell, t_memory *current,
		t_memory *prev, t_memory *next)
{
	free(current->addr);
	free(current);
	if (prev)
		prev->next = next;
	else
		shell->memory = next;
}

void	free_non_heredoc_memory(t_shell *shell)
{
	t_memory	*current;
	t_memory	*next;
	t_memory	*prev;

	if (!shell || !shell->memory)
		return ;
	current = shell->memory;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (!is_heredoc_memory(shell, current->addr)
			&& !is_a_env_memory(shell, current->addr))
			free_memory_node(shell, current, prev, next);
		else
			prev = current;
		current = next;
	}
}
