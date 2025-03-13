/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:22:42 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/13 08:34:13 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_echo_options(char **args, int *index)
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
	*index = i;
	return (n_option);
}

static void	write_echo_content(char **args, int i)
{
	char	space[2];

	space[0] = ' ';
	space[1] = '\0';
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, space, 1);
		i++;
	}
}

int	builtin_echo(char **args)
{
	int		i;
	int		n_option;
	char	*dummy;
	char	newline[2];

	newline[0] = '\n';
	newline[1] = '\0';
	n_option = process_echo_options(args, &i);
	write_echo_content(args, i);
	if (!n_option)
		write(STDOUT_FILENO, newline, 1);
	write(STDOUT_FILENO, &dummy, 0);
	return (0);
}

int	builtin_pwd(void)
{
	char	current_path[1024];
	char	newline[2];

	newline[0] = '\n';
	newline[1] = '\0';
	if (getcwd(current_path, sizeof(current_path)))
	{
		write(STDOUT_FILENO, current_path, ft_strlen(current_path));
		write(STDOUT_FILENO, newline, 1);
		return (0);
	}
	return (1);
}
