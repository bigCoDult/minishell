/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:22:42 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/09 11:47:08 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	j;
	int	n_option;

	n_option = 0;
	i = 1;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (j > 1 && args[i][j] == '\0')
		{
			n_option = 1;
			i++;
		}
		else
			break ;
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
	fflush(stdout);
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




