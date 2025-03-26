#include "../../inc/minishell.h"

static int	redirect_input(t_global *g, int id)
{
	if (g->cmds[id]->infile != -1)
	{
		if (id != 0)
			close(g->cmds[id]->prev_fd);
		if (dup2(g->cmds[id]->infile, 0) == -1)
			return (1);
	}
	else if (id != 0)
	{
		if (dup2(g->cmds[id]->prev_fd, 0) == -1)
		{
			close(g->cmds[id]->prev_fd);
			return (1);
		}
		close(g->cmds[id]->prev_fd);
	}
	return (0);
}

static int	redir_output(t_global *g, int id)
{
	if (g->cmds[id]->outfile != -1)
	{
		if (dup2(g->cmds[id]->outfile, 1) == -1)
			return (1);
	}
	else if (g->cmds[id + 1])
	{
		if (dup2(g->cmds[id]->pipe[1], 1) == -1)
			return (1);
	}
	close(g->cmds[id]->pipe[1]);
	return (0);
}

static void	exec_pipe_child(t_global *g, int id)
{
	if (!prep_cmd_exec(g, id))
	{
		if (redirect_input(g, id) || redir_output(g, id))
		{
			(close(g->cmds[id]->pipe[1]));
			free_g(g, NULL);
			ft_perror("Exec: error with dup\n", 0);
			exit(1);
		}
		close_child_fds(g);
		if (dispatch_cmd(g, g->cmds[id]) == NONE)
			exec_cmd(g, id);
	}
	(free_g(g, NULL), exit(g->exit_val));
}

static void	spawn_pipe_child(t_global *g, int id)
{
	pid_t	pid;

	if (pipe(g->cmds[id]->pipe) == -1)
		return ;
	pid = fork();
	if (pid == -1)
		ft_perror("Error: fork pipes\n", 0);
	else if (pid == 0)
	{
		close(g->cmds[id]->pipe[0]);
		exec_pipe_child(g, id);
	}
	g->last_pid = pid;
	if (g->cmds[id]->infile > 2)
		close(g->cmds[id]->infile);
	if (g->cmds[id]->outfile > 2)
		close(g->cmds[id]->outfile);
	return ;
}

void	exec_pipes(t_global *g)
{
	int		i;

	i = 0;
	while (g->cmds[i])
	{
		spawn_pipe_child(g, i);
		close(g->cmds[i]->pipe[1]);
		if (g->cmds[i + 1])
		{
			g->cmds[i + 1]->prev_fd = g->cmds[i]->pipe[0];
			if (g->cmds[i]->prev_fd != -1)
				close(g->cmds[i]->prev_fd);
		}
		else
			break ;
		i++;
	}
	close(g->cmds[i]->pipe[0]);
	if (g->cmds[i]->prev_fd != -1)
		close(g->cmds[i]->prev_fd);
	ft_wait_for_childs(g);
}
