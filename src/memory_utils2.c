/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 04:34:29 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/20 14:29:32 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*shell_malloc(t_shell *shell, size_t size)
{
	void		*ptr;
	t_memory	*new_node;

	if (!shell || size == 0)
		return (NULL);
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

	if (!ptr || !shell)
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
