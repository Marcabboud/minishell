#include "../../inc/minishell.h"

t_state	is_format_export(char *str, bool *equal)
{
	int	i;

	*equal = false;
	if (!((str[0] >= 'a' && str[0] <= 'z') \
	|| (str[0] >= 'A' && str[0] <= 'Z') || str[0] == '_'))
		return (ERROR);
	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
		{
			*equal = true;
			return (VALID);
		}
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (ERROR);
		i++;
	}
	return (NONE);
}

static void	print_export(t_env *lenv)
{
	t_env	*tmp;
	char	*res;

	tmp = lenv;
	while (tmp != NULL)
	{
		res = remq(ft_strdup(tmp->content));
		printf("export %s=\"%s\"\n", tmp->name, res);
		free(res);
		tmp = tmp->next;
	}
}

static bool	handle_existing_var(t_env *tmp, char *e_name, char *str, int i)
{
	if (ft_strcmp(e_name, tmp->name))
	{
		free(tmp->content);
		tmp->content = remq(ft_substr(str, i + 1, ft_strlen(str)));
		free(e_name);
		return (true);
	}
	return (false);
}

static bool	single_export(t_global *g, char *str, int i)
{
	char	*e_name;
	t_env	*tmp;
	bool	equal;

	if (is_format_export(str, &equal) == ERROR)
		return (g->exit_val = 1, ft_perror(EID, 0), false);
	if (equal == false)
		return (true);
	tmp = g->lenv;
	while (str[i] && str[i] != '=')
		i++;
	e_name = ft_substr(str, 0, i);
	while (tmp)
	{
		if (handle_existing_var(tmp, e_name, str, i))
			return (true);
		if (!tmp->next)
			break ;
		tmp = tmp->next;
	}
	if (!ft_strcmp(e_name, tmp->name))
		tmp->next = create_node(str);
	free(e_name);
	return (true);
}

bool	ft_export(t_global *g, char **str)
{
	t_state	res;
	int		i;

	if (!*str)
		return (print_export(g->lenv), true);
	i = 0;
	while (str[i])
	{
		res = is_format_export(str[i], &(bool){0});
		if (res == ERROR || res == NONE)
		{
			if (res == ERROR && g->exit_val == 0)
				(ft_perror(EID, 0), g->exit_val = 1);
			i++;
			continue ;
		}
		single_export(g, str[i++], 0);
	}
	return (VALID);
}
