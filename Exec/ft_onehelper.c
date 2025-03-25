#include "../../inc/minishell.h"

int	close_fd(t_global *g, int *std_save, bool is_error)
{
	if (g->cmds[std_save[2]]->infile != -1)
	{
		if (dup2(std_save[0], 0) == -1 || is_error)
			return (handle_fd_error(g, std_save, is_error));
		close(std_save[0]);
	}
	if (g->cmds[std_save[2]]->outfile != -1)
	{
		if (dup2(std_save[1], 1) == -1 || is_error)
			return (handle_out_error(g, std_save, is_error));
		close(std_save[1]);
	}
	return (1);
}

int	handle_fd_error(t_global *g, int *std_save, bool is_error)
{
	close(std_save[0]);
	if (g->cmds[std_save[2]]->outfile != -1)
		close(std_save[1]);
	if (is_error)
		(free_g(g, NULL), exit(1));
	return (1);
}

int	handle_out_error(t_global *g, int *std_save, bool is_error)
{
	close(std_save[1]);
	if (is_error)
		(free_g(g, NULL), exit(1));
	return (1);
}

void	gest_shell(t_global *g, int id, int *std_save)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (ft_perror("Error :Fork failed\n", 0),
			free_g(g, NULL), exit(0), (void)0);
	if (pid == 0)
	{
		close_child_fd(g, id, std_save);
		execve_cmd(g, id);
	}
	g->last_pid = pid;
	ft_waitall(g);
}

void	close_child_fd(t_global *g, int id, int *std_save)
{
	if (g->cmds[id]->infile != -1)
	{
		close(std_save[0]);
		close(g->cmds[id]->infile);
	}
	if (g->cmds[id]->outfile != -1)
	{
		close(std_save[1]);
		close(g->cmds[id]->outfile);
	}
}
