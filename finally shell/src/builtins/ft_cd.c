/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabboud <mabboud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:20:46 by mabboud           #+#    #+#             */
/*   Updated: 2025/03/28 13:24:51 by mabboud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	cd_home(t_global *g, char *env_value, char *tmp)
{
	env_value = get_env_content(g->lenv, "HOME");
	if (!env_value)
	{
		(free(env_value), ft_perror("cd: HOME not set\n", 0));
		g->exit_val = 1;
		return (false);
	}
	(chdir(env_value), free(env_value));
	env_value = get_env_content(g->lenv, "PWD");
	tmp = pwrapper("OLDPWD", env_value, '=');
	(free(env_value), ft_export(g, &tmp, false), free(tmp));
	env_value = get_env_content(g->lenv, "HOME");
	tmp = pwrapper("PWD", env_value, '=');
	return (free(env_value), ft_export(g, &tmp, false), free(tmp), true);
}

bool	ft_cd(t_global *g, char **arr)
{
	char	cwd[1024];
	char	*tmp;
	char	*pl;

	tmp = NULL;
	pl = NULL;
	if (!arr[1] || ft_strcmp(arr[1], "~"))
		return (cd_home(g, pl, tmp));
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
