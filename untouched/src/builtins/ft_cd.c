/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eden <eden@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 03:20:08 by redrouic          #+#    #+#             */
/*   Updated: 2025/03/26 09:49:29 by eden             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	simple_cd(t_global *g, char *pl, char *tmp)
{
	pl = get_env_content(g->lenv, "HOME");
	if (!pl)
	{
		(free(pl), ft_perror("cd: HOME not set\n", 0));
		g->exit_val = 1;
		return (false);
	}
	(chdir(pl), free(pl));
	pl = get_env_content(g->lenv, "PWD");
	tmp = pwrapper("OLDPWD", pl, '=');
	(free(pl), ft_export(g, &tmp, false), free(tmp));
	pl = get_env_content(g->lenv, "HOME");
	tmp = pwrapper("PWD", pl, '=');
	return (free(pl), ft_export(g, &tmp, false), free(tmp), true);
}

bool	ft_cd(t_global *g, char **arr)
{
	char	cwd[1024];
	char	*tmp;
	char	*pl;

	tmp = NULL;
	pl = NULL;
	if (!arr[1] || ft_strcmp(arr[1], "~"))
		return (simple_cd(g, pl, tmp));
	else if (arr[2])
		return (g->exit_val = 1
			, ft_perror(" too many arguments\n", 0), false);
	else if (chdir(arr[1]) == -1)
		return (g->exit_val = 1, perror("cd"), false);
	else if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		pl = get_env_content(g->lenv, "PWD");
		tmp = pwrapper("OLDPWD", pl, '=');
		(free(pl), ft_export(g, &tmp, false), free(tmp));
		tmp = pwrapper("PWD", cwd, '=');
		return (ft_export(g, &tmp, false), free(tmp), true);
	}
	return (perror("getcwd"), true);
}
