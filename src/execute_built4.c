/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:02:50 by sanbaek           #+#    #+#             */
/*   Updated: 2025/03/18 18:51:57 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_env(t_env *env_head, char *var, char *error_msg, int print_cwd)
{
	char	*p;
	char	buf[4096];

	p = get_env(env_head, var);
	if (p == NULL || (print_cwd && chdir(p) != 0))
	{
		printf("%s\n", error_msg);
		return (1);
	}
	if (print_cwd)
	{
		getcwd(buf, 4096);
		printf("%s\n", buf);
	}
	else
		chdir(p);
	return (0);
}

static int	cd_special_path(t_tree *node, t_env *env_head)
{
	if (node->left_child == NULL || \
		ft_strcmp(node->left_child->value, "~") == 0)
		return (cd_env(env_head, "HOME", "cd: HOME not set", 0));
	if (ft_strcmp(node->left_child->value, "-") == 0)
		return (cd_env(env_head, "OLDPWD", "cd: OLDPWD not set", 1));
	return (0);
}

void	handle_cd_command(t_tree *node, t_env *env_head, t_shell *shell)
{
	char	oldpwd[4096];
	char	newpwd[4096];

	ft_memset(oldpwd, 0, 4096);
	ft_memset(newpwd, 0, 4096);
	if (node == NULL)
		return ;
	getcwd(oldpwd, 4096);
	if (node->left_child == NULL || !ft_strcmp(node->left_child->value, "~") || \
		!ft_strcmp(node->left_child->value, "-"))
	{
		shell->status.exit_code = cd_special_path(node, env_head);
		if (shell->status.exit_code == 1)
			return ;
	}
	else if (chdir(node->left_child->value) != 0)
	{
		shell->status.exit_code = 1;
		perror("cd");
		return ;
	}
	getcwd(newpwd, 4096);
	export_for_cd("OLDPWD", oldpwd, &shell->env, shell);
	export_for_cd("PWD", newpwd, &shell->env, shell);
	shell->status.exit_code = 0;
}

t_tree	*convert_args_to_node(char **args, t_tree *node)
{
	int	i;

	i = 0;
	node[i].value = args[i];
	node[i].right_sibling = NULL;
	if (args[i + 1])
		node[i].left_child = &node[i + 1];
	else
		node[i].left_child = NULL;
	i++;
	while (args[i])
	{
		node[i].value = args[i];
		node[i].left_child = NULL;
		if (args[i + 1])
			node[i].right_sibling = &node[i + 1];
		else
			node[i].right_sibling = NULL;
		i++;
	}
	return (&node[0]);
}

int	builtin_cd(t_shell *shell, char **args)
{
	t_tree	node[100];
	int		count;

	count = 0;
	while (args[count])
	{
		count++;
	}
	if (count > 2)
	{
		printf("cd: too many arguments\n");
		shell->status.exit_code = 1;
		return (1);
	}
	convert_args_to_node(args, node);
	handle_cd_command(node, shell->env, shell);
	return (0);
}
