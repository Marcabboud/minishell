/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabboud <mabboud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:20:50 by mabboud           #+#    #+#             */
/*   Updated: 2025/03/28 13:24:51 by mabboud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static inline bool	is_alpha(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

t_state	format_export(char *str)
{
	bool	equal;
	int		i;

	equal = false;
	if (!is_alpha(str[0]) && str[0] != '_')
		return (ERROR);
	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
		{
			equal = true;
			break ;
		}
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (ERROR);
		i++;
	}
	if (!equal)
		return (NONE);
	return (VALID);
}

static void	print_env(t_env *env_list)
{
	t_env	*tmp;
	char	*unquoted_val;

	tmp = env_list;
	while (tmp != NULL)
	{
		unquoted_val = remove_quotes(ft_strdup(tmp->content));
		printf("export %s=\"%s\"\n", tmp->name, unquoted_val);
		free(unquoted_val);
		tmp = tmp->next;
	}
	tmp = env_list;
}

static	bool	one_env(t_global *g, char *str, int i, t_env *tmp)
{
	char	*env_name;

	if (format_export(str) == NONE)
		return (true);
	if (format_export(str) == ERROR)
		return (g->exit_val = 1, ft_perror(INVALID_IDENTIFIER, 0), false);
	while (str[i] && str[i] != '=')
		i++;
	env_name = ft_substr(str, 0, i);
	while (tmp)
	{
		if (ft_strcmp(env_name, tmp->name))
		{
			free(tmp->content);
			tmp->content = remove_quotes(ft_substr(str, i + 1, ft_strlen(str)));
			return (free(env_name), true);
		}
		if (!tmp->next)
			break ;
		tmp = tmp->next;
	}
	if (!ft_strcmp(env_name, tmp->name))
		tmp->next = create_node(str);
	return (free(env_name), true);
}

bool	ft_export(t_global *g, char **str, bool arg_nb)
{
	t_env	*tmp;
	t_state	export_state;
	int		i;

	tmp = g->lenv;
	i = 0;
	if (!*str)
		return (print_env(g->lenv), true);
	if (!arg_nb)
		return (one_env(g, *str, 0, tmp), VALID);
	while (str[i] != NULL)
	{
		export_state = format_export(str[i]);
		if (export_state == ERROR || export_state == NONE)
		{
			if (export_state == ERROR && g->exit_val == 0)
				(ft_perror(INVALID_IDENTIFIER, 0), g->exit_val = 1);
			i++;
			continue ;
		}
		one_env(g, str[i++], 0, tmp);
	}
	return (VALID);
}
