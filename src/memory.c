/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:24:40 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/19 10:18:38 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_env_memory(t_shell *shell, void *addr)
{
	t_env	*env_ptr;

	if (!shell || !shell->env || !addr)
		return (0);
		
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

	if (!shell || !shell->memory)
		return ;
		
	current = shell->memory;
	prev = NULL;
	while (current)
	{
		next = current->next;
		// 환경 변수 메모리는 free_env에서 처리하므로 제외
		if (!is_env_memory(shell, current->addr))
		{
			if (current->addr)
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

void	free_shell_malloc(t_shell *shell)
{
	t_memory	*current;
	t_memory	*next;

	if (!shell || !shell->memory)
		return ;
		
	current = shell->memory;
	while (current)
	{
		next = current->next;
		if (current->addr)
			free(current->addr);
		free(current);
		current = next;
	}
	shell->memory = NULL;
}

void	free_env(t_shell *shell)
{
	t_env	*current;
	t_env	*next;

	if (!shell || !shell->env)
		return ;
		
	current = shell->env;
	while (current)
	{
		next = current->next;
		
		// 메모리 주소가 shell->memory에 있는지 확인
		if (current->key)
			shell_free(shell, current->key);
		if (current->value)
			shell_free(shell, current->value);
			
		// 환경 변수 노드 자체 해제
		shell_free(shell, current);
		
		current = next;
	}
	shell->env = NULL;
}

void	free_exit(t_shell *shell, int status)
{
	if (!shell)
		exit(status);
		
	// 먼저 환경 변수 메모리 해제 (값과 키가 먼저 해제되어야 함)
	free_env(shell);
	
	// 그 다음 다른 모든 메모리 해제
	free_shell_malloc(shell);
	
	exit(status);
}
