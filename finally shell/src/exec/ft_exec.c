/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabboud <mabboud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:21:03 by mabboud           #+#    #+#             */
/*   Updated: 2025/03/28 13:24:51 by mabboud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	update_last_cmd(t_global *g, t_cmd *cmd)
{
	char	*new_val;
	int		len;
	char	*old_val;

	if (access(cmd->args[0], X_OK) == 0)
	{
		new_val = ft_strdup(cmd->args[0]);
		old_val = pwrapper("_", new_val, '=');
		ft_export(g, &old_val, false);
		return (free(new_val), free(old_val), (void)0);
	}
	len = (11 + ft_strlen(cmd->args[0]) + 1);
	new_val = malloc(sizeof(char) * len);
	if (!new_val)
		return (ft_perror(ERR_ALLOCATION, 0), (void)0);
	ft_strncpy(new_val, "_=/usr/bin/", 12);
	ft_strlcat(new_val, cmd->args[0], len);
	old_val = get_env_content(g->lenv, "_");
	if (old_val)
	{
		ft_export(g, &new_val, false);
		free(old_val);
	}
	free(new_val);
}

void	close_child_fds(t_global *g)
{
	int	i;

	i = 0;
	while (g->cmds[i])
	{
		if (g->cmds[i]->infile > 2)
			close(g->cmds[i]->infile);
		if (g->cmds[i]->outfile > 2)
			close(g->cmds[i]->outfile);
		i++;
	}
}

int	prep_cmd_exec(t_global *g, int id)
{
	if (g->cmds[id] && g->cmds[id]->args && g->cmds[id]->args[0]
		&& ft_strlen(g->cmds[id]->args[0]) != 0)
	{
		if (ft_strcmp(g->cmds[0]->args[0], "exit"))
			return (0);
		update_last_cmd(g, g->cmds[id]);
		if (g->cmds[id]->infile != -2 && g->cmds[id]->outfile != -2)
			return (0);
	}
	if (g->cnt > 1)
	{
		close_child_fds(g);
		if (id != 0)
			close(g->cmds[id]->prev_fd);
		close(g->cmds[id]->pipe[1]);
	}
	return (1);
}

int	ft_exec(t_global *g)
{
	g_signal = 0;
	if (!g->cnt)
		return (0);
	if (g->cnt == 1)
		return (execute_cmd(g, 0));
	else
		exec_pipes(g);
	return (0);
}
