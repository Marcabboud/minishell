#include "../../inc/minishell.h"

static char	*fetch_path(t_env *lenv, char **arr)
{
	char	**tab;
	char	*path;
	int		i;

	path = plist(lenv, "PATH");
	if (!path)
		return (NULL);
	tab = str2arr(path, ":", false);
	free(path);
	if (!tab)
		return (NULL);
	i = 0;
	while (tab[i])
	{
		path = pwrapper(tab[i++], arr[0], '/');
		if (path && access(path, X_OK) == 0)
		{
			free_arr(tab);
			return (path);
		}
		free(path);
	}
	free_arr(tab);
	return (NULL);
}

static void	handle_permission(char *cmd, char **lenv)
{
	if (access(cmd, X_OK) != 0)
	{
		ft_perror(cmd, 0);
		ft_perror(": Permission denied\n", 0);
	}
	else
	{
		ft_perror(cmd, 0);
		ft_perror(": Is a directory\n", 0);
	}
	free_g(NULL, lenv);
	exit(126);
}

void	execve_absolute_path(t_global *g, int id)
{
	char	**lenv;
	int		access_check;

	lenv = list2arr(g->lenv);
	access_check = access(g->cmds[id]->args[0], F_OK);
	if (execve(g->cmds[id]->args[0], g->cmds[id]->args, lenv) == -1)
	{
		if (access_check == 0)
			handle_permission(g->cmds[id]->args[0], lenv);
		perror(g->cmds[id]->args[0]);
		free_g(g, lenv);
		exit(127);
	}
}

void	execve_cmd_path(t_global *g, int id)
{
	char	*path;
	char	**lenv;

	path = fetch_path(g->lenv, g->cmds[id]->args);
	lenv = list2arr(g->lenv);
	if (!path)
	{
		ft_perror(g->cmds[id]->args[0], 0);
		ft_perror(": command not found\n", 0);
		free_g(g, lenv);
		exit(127);
	}
	if (execve(path, g->cmds[id]->args, lenv) == -1)
	{
		perror("execve");
		free_g(g, lenv);
		free(path);
		exit(1);
	}
}

void	execve_cmd(t_global *g, int id)
{
	int	i;

	if (!g->cmds[id])
	{
		if (g->cmds[id + 1])
			execve_cmd(g, id + 1);
		return ;
	}
	i = 0;
	while (g->cmds[id]->args[0][i])
	{
		if (g->cmds[id]->args[0][i] == '/')
		{
			execve_absolute_path(g, id);
			return ;
		}
		i++;
	}
	execve_cmd_path(g, id);
}
