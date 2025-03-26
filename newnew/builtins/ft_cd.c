#include "../../inc/minishell.h"

static bool	simple_cd(t_global *g, char *env_val, char *export_str)
{
	env_val = plist(g->lenv, "HOME");
	if (!env_val)
	{
		free(env_val);
		ft_perror("cd: HOME not set\n", 0);
		g->exit_val = 1;
		return (false);
	}
	chdir(env_val);
	free(env_val);
	env_val = plist(g->lenv, "PWD");
	export_str = pwrapper("OLDPWD", env_val, '=');
	free(env_val);
	ft_export(g, &export_str, false);
	free(export_str);
	env_val = plist(g->lenv, "HOME");
	export_str = pwrapper("PWD", env_val, '=');
	free(env_val);
	ft_export(g, &export_str, false);
	free(export_str);
	return (true);
}

bool	ft_cd(t_global *g, char **args)
{
	char	current_dir[1024];
	char	*export_str;
	char	*env_val;

	export_str = NULL;
	env_val = NULL;
	if (!args[1] || ft_strcmp(args[1], "~"))
		return (simple_cd(g, env_val, export_str));
	else if (args[2])
		return (g->exit_val = 1, ft_perror(" too many arguments\n", 0), false);
	else if (chdir(args[1]) == -1)
		return (g->exit_val = 1, perror("cd"), false);
	else if (getcwd(current_dir, sizeof(current_dir)) != NULL)
	{
		env_val = plist(g->lenv, "PWD");
		export_str = pwrapper("OLDPWD", env_val, '=');
		free(env_val);
		ft_export(g, &export_str, false);
		free(export_str);
		export_str = pwrapper("PWD", current_dir, '=');
		return (ft_export(g, &export_str, false), free(export_str), true);
	}
	return (perror("getcwd"), true);
}
