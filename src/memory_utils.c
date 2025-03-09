/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:37:51 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/08 12:41:25 by yutsong          ###   ########.fr       */
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

void	free_non_heredoc_memory(t_shell *shell)
{
	t_memory	*current;
	t_memory	*next;
	t_memory	*prev;
	t_env		*env_ptr;
	
	if (!shell || !shell->memory)
		return ;
	
	current = shell->memory;
	prev = NULL;
	env_ptr = shell->env;
	
	while (current)
	{
		next = current->next;
		
		// 히어독 관련 메모리인지 확인
		int is_heredoc_memory = 0;
		if (shell->heredoc.entries)
		{
			t_heredoc_entry *entry = shell->heredoc.entries;
			// 히어독 엔트리와 관련된 메모리인지 확인
			if (current->addr == entry || 
			    (current->addr >= (void *)entry && 
			     current->addr < (void *)entry + sizeof(t_heredoc_entry) * shell->heredoc.count))
			{
				is_heredoc_memory = 1;
			}
		}
		
		// env와 히어독 관련 메모리는 보존
		if (current->addr != env_ptr && 
		    (current->addr < (void *)env_ptr || 
		     current->addr > (void *)(env_ptr + sizeof(t_env))) &&
		    !is_heredoc_memory)
		{
			free(current->addr);
			free(current);
			if (prev)
				prev->next = next;
			else
				shell->memory = next;
		}
		else
		{
			prev = current;
		}
		
		current = next;
	}
}
