/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eden <eden@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 01:34:17 by redrouic          #+#    #+#             */
/*   Updated: 2025/03/26 09:49:29 by eden             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*find_path(t_env *env_list, char **arr)
{
	char	**path_tokens;
	char	*path;
	int		i;

	i = 0;
	path = get_env_content(env_list, "PATH");
	if (!path)
		return (NULL);
	path_tokens = str_to_arr(path, ":", false);
	free(path);
	if (!path_tokens)
		return (NULL);
	path = NULL;
	while (path_tokens[i] != NULL)
	{
		path = pwrapper(path_tokens[i++], arr[0], '/');
		if (!path)
			return (free_arr(path_tokens), NULL);
		if (access(path, X_OK) == 0)
			return (free_arr(path_tokens), path);
		free(path);
		path = NULL;
	}
	return (free_arr(path_tokens), NULL);
}

static void	handle_exec_error(t_global *g, const char *cmd, char **env_arr)
{
	if (access(cmd, F_OK) == 0)
	{
		ft_perror(cmd, 0);
		if (access(cmd, X_OK) != 0)
			ft_perror(": Permission denied\n", 0);
		else
			ft_perror(": Is a directory\n", 0);
		g->exit_val = 126;
	}
	else
	{
		perror(cmd);
		g->exit_val = 127;
	}
	free_g(g, env_arr);
	exit(g->exit_val);
}

void	execve_absolute_path(t_global *g, int id)
{
	char	**env_arr;
	char	*cmd;
	char	**argv;

	env_arr = env_list_to_arr(g->lenv);
	cmd = g->cmds[id]->args[0];
	argv = g->cmds[id]->args;
	if (execve(cmd, argv, env_arr) == -1)
		handle_exec_error(g, cmd, env_arr);
}

void	execve_cmd_path(t_global *g, int id)
{
	char	*path;
	char	**lenv;
	int		i;

	i = 0;
	while (g->cmds[id]->args[0][i] && is_chars(".'\"", g->cmds[id]->args[0][i]))
		i++;
	if (g->cmds[id]->args[0][i] == '\0')
		path = NULL;
	else
		path = find_path(g->lenv, g->cmds[id]->args);
	lenv = env_list_to_arr(g->lenv);
	if (!path)
	{
		ft_perror(g->cmds[id]->args[0], 0);
		ft_perror(": command not found\n", 0);
		(free_g(g, lenv), exit(127));
	}
	if (execve(path, g->cmds[id]->args, lenv) == -1)
	{
		perror("execve");
		(free_g(g, lenv), free(path), exit(1));
	}
}

void	execve_cmd(t_global *g, int id)
{
	int		i;

	if (g->cmds[id] == NULL)
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
			exec_abs_path(g, id);
			return ;
		}
		i++;
	}
	execve_cmd_path(g, id);
}
