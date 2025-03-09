#include "minishell.h"

void	add_keyvalue(t_env *input_env, t_env *env_head)
{
	t_env	*current;

	if (!input_env || !env_head)
		return ;
	if (!(input_env->key) || !(input_env->value))
		return ;
	current = env_head;
	while (current)
	{
		if (current->key && (ft_strcmp(current->key, input_env->key) == 0))
		{
			current->value = ft_strdup(input_env->value);
			return ;
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	add_env_node(&env_head, input_env);
}

char	*get_env(t_env *head, char *key)
{
	t_env	*current;

	current = head;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

t_env	*find_already(char *key, t_env *env_head)
{
	t_env	*current;

	current = env_head;
	while (current)
	{
		if (current->key && (ft_strcmp(current->key, key) == 0))
			return (current);
		current = current->next;
	}
	return (NULL);
}

static int	cd_special_path(t_tree *node, t_env *env_head)
{
	char	newpwd[4096];
	char	*path;

	if (ft_strcmp(node->value, "cd") != 0)
		return (0);
	if (node->left_child == NULL)
	{
		path = get_env(env_head, "HOME");
		if (path == NULL)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
		chdir(path);
		return (1);
	}
	else if (ft_strcmp(node->left_child->value, "~") == 0)
	{
		path = get_env(env_head, "HOME");
		if (path == NULL)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
		chdir(path);
		return (1);
	}
	else if (ft_strcmp(node->left_child->value, "-") == 0)
	{
		path = get_env(env_head, "OLDPWD");
		if (path == NULL || chdir(path) != 0)
		{
			fprintf(stderr, "cd: OLDPWD not set\n");
			return (0);
		}
		getcwd(newpwd, 4096);
		printf("%s\n", newpwd);
		return (1);
	}
	else
		return (0);
}

static void	export_for_cd(char *key, char *value, t_env **env_head, t_shell *shell)
{
	if (!find_already(key, *env_head))
		add_env_value(shell, key, value);
	else
		set_env_value(shell, key, value);
}

void	handle_cd_command(t_tree *node, t_env *env_head, t_shell *shell)
{
	char	oldpwd[4096];
	char	newpwd[4096];

	if (node == NULL)
		return ;
	getcwd(oldpwd, 4096);
	if (cd_special_path(node, env_head))
		;
	else if (chdir(node->left_child->value) != 0)
	{
		perror("cd");
		return ;
	}
	getcwd(newpwd, 4096);
	export_for_cd("OLDPWD", oldpwd, &shell->env, shell);
	export_for_cd("PWD", newpwd, &shell->env, shell);
}

t_tree *convert_args_to_node(char **args, t_tree *node)
{
	int	i;
	
	i = 0;
	while (args[i])
	{
		node[i].value = args[i];
		if (args[i + 1])
			node[i].left_child = &node[i + 1];
		else
			node[i].left_child = NULL;
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
		return (1);
	}
	convert_args_to_node(args, node);
	handle_cd_command(node, shell->env, shell);
	return (0);
}
