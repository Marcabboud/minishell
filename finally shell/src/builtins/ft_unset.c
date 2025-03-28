/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabboud <mabboud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:20:53 by mabboud           #+#    #+#             */
/*   Updated: 2025/03/28 13:24:51 by mabboud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	remove_first_node(t_env **lenv)
{
	t_env	*to_delete;

	to_delete = *lenv;
	*lenv = (*lenv)->next;
	ft_free_node(to_delete);
}

static bool	check_and_remove(t_env **tmp, char *var)
{
	t_env	*to_delete;

	if (ft_strcmp(var, (*tmp)->next->name))
	{
		to_delete = (*tmp)->next;
		(*tmp)->next = (*tmp)->next->next;
		ft_free_node(to_delete);
		return (true);
	}
	return (false);
}

static void	unset_var(t_env **lenv, char *var)
{
	t_env	*tmp;

	tmp = *lenv;
	if (ft_strcmp(var, (*lenv)->name))
		return (remove_first_node(lenv));
	while (tmp && tmp->next)
	{
		if (check_and_remove(&tmp, var))
			return ;
		tmp = tmp->next;
	}
}

void	ft_unset(t_global *g, char **str)
{
	int	i;

	if (!g->lenv || !str)
		return ;
	g->exit_val = 0;
	i = 0;
	while (str[i] != NULL)
		unset_var(&g->lenv, str[i++]);
}
