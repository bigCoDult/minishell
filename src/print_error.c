/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutsong <yutsong@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 05:43:41 by yutsong           #+#    #+#             */
/*   Updated: 2025/03/15 08:25:09 by yutsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(const char *format, ...)
{
	va_list	args;

	va_start(args, format);
	if (ft_strcmp(format, "minishell: %s: No such file or directory\n") == 0)
		handle_file_not_found(args);
	else if (ft_strcmp(format, "minishell: %s: Permission denied\n") == 0)
		handle_permission_denied(args);
	else if (ft_strcmp(format, "minishell: %s: command not found\n") == 0)
		handle_command_not_found(args);
	else if (ft_strcmp(format, "minishell: %s: %s\n") == 0)
		handle_custom_error(args);
	else if (ft_strcmp(format,
			"minishell: export: `%s': not valid identifier\n") == 0)
		handle_export_error(args);
	else
		write(STDERR_FILENO, format, ft_strlen(format));
	va_end(args);
}
