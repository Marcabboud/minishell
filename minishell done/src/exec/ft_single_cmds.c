#include "../../inc/minishell.h"

void	ft_wait_for_childs(t_global *g)
{
	int	pid;
	int	child_status;

	while (42)
	{
		pid = wait(&child_status);
		if (pid == g->last_pid)
		{
			if (WIFEXITED(child_status))
				g->exit_val = WEXITSTATUS(child_status);
		}
		if (g_signal != 0)
			g->exit_val = 128 + g_signal;
		if (pid == -1)
			break ;
	}
}

int	restore_fds(t_global *g, int *saved_fds, bool is_error)
{
	if (g->cmds[saved_fds[2]]->infile != -1)
	{
		if (dup2(saved_fds[0], 0) == -1 || is_error)
		{
			close(saved_fds[0]);
			if (g->cmds[saved_fds[2]]->outfile != -1)
				close(saved_fds[1]);
			if (is_error)
				(free_g(g, NULL), exit(1));
		}
		close(saved_fds[0]);
	}
	if (g->cmds[saved_fds[2]]->outfile != -1)
	{
		if (dup2(saved_fds[1], 1) == -1 || is_error)
		{
			close(saved_fds[1]);
			if (is_error)
				(free_g(g, NULL), exit(1));
		}
		close(saved_fds[1]);
	}
	return (1);
}

static int	redirect_io(t_global *g, int *saved_fds)
{
	if (g->cmds[saved_fds[2]]->infile != -1)
	{
		saved_fds[0] = dup(0);
		if (saved_fds[0] == -1)
			return (1);
		if (dup2(g->cmds[saved_fds[2]]->infile, 0) == -1)
		{
			close(saved_fds[0]);
			return (1);
		}
	}
	if (g->cmds[saved_fds[2]]->outfile != -1)
	{
		saved_fds[1] = dup(1);
		if (saved_fds[1] == -1)
			return (restore_fds(g, saved_fds, true));
		if (dup2(g->cmds[saved_fds[2]]->outfile, 1) == -1)
			return (restore_fds(g, saved_fds, true));
	}
	return (0);
}

static void	exec_child_cmd(t_global *g, int id, int *saved_fds)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (ft_perror("Error :Fork failed\n", 0),
			free_g(g, NULL), exit(0), (void)0);
	else if (pid == 0)
	{
		if (g->cmds[id]->infile != -1)
			(close(saved_fds[0]), close(g->cmds[id]->infile));
		if (g->cmds[id]->outfile != -1)
			(close(saved_fds[1]), close(g->cmds[id]->outfile));
		exec_cmd(g, id);
	}
	g->last_pid = pid;
	ft_wait_for_childs(g);
}

int	execute_cmd(t_global *g, int id)
{
	int		saved_fds[3];

	if (prep_cmd_exec(g, 0))
		return (1);
	saved_fds[2] = id;
	if (redirect_io(g, saved_fds) == 1)
	{
		ft_perror("Exec: error with dup\n", 0);
		g->exit_val = 1;
		return (g->exit_val);
	}
	if (dispatch_cmd(g, g->cmds[id]) == NONE)
		exec_child_cmd(g, id, saved_fds);
	restore_fds(g, saved_fds, false);
	return (g->exit_val);
}
