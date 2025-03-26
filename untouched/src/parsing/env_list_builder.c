/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_builder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eden <eden@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:22:15 by gmassin           #+#    #+#             */
/*   Updated: 2025/03/26 09:24:16 by eden             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_env	*create_node(char *str)
{
	t_env	*node;
	int		i;

	if (!str)
		return (NULL);
	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	node->name = ft_substr(str, 0, i);
	node->content = ft_substr(str, i + 1, ft_strlen(str));
	node->next = NULL;
	if (!node->name || !node->content)
	{
		ft_free_node(node);
		return (NULL);
	}
	return (node);
}

static char	**new_env(void)
{
	char	current_dir[1024];
	char	**env_array;

	env_array = malloc(sizeof(char *) * 4);
	if (!env_array)
		return (NULL);
	getcwd(current_dir, sizeof(current_dir));
	env_array[0] = pwrapper("PWD", current_dir, '=');
	env_array[1] = ft_strdup("_=/usr/bin/env");
	env_array[2] = ft_strdup("SHLVL=1");
	env_array[3] = NULL;
	return (env_array);
}

t_env	*env_arr_to_list(char **env)
{
	t_env	*dummy;
	t_env	*current;
	t_env	*result;

	if (!*env)
		env = new_env();
	dummy = create_node("a=b");
	if (!dummy)
		return (NULL);
	current = dummy;
	while (*env)
	{
		current->next = create_node(*env);
		if (!current->next)
		{
			free_list(dummy);
			return (NULL);
		}
		current = current->next;
		env++;
	}
	result = dummy->next;
	ft_free_node(dummy);
	return (result);
}

char	**env_list_to_arr(t_env *lenv)
{
	t_env	*current_node;
	char	**env_array;
	int		i;

	i = 0;
	current_node = lenv;
	while (current_node)
	{
		i++;
		current_node = current_node->next;
	}
	env_array = malloc(sizeof(char *) * (i + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	current_node = lenv;
	while (current_node)
	{
		env_array[i++] = pwrapper(current_node->name,
				current_node->content, '=');
		current_node = current_node->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
