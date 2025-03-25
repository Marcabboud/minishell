#include "../../inc/minishell.h"

void	ft_waitall(t_global *g)
{
	int	pid;
	int	exit_status;

	while (42)
	{
		pid = wait(&exit_status);
		if (pid == g->last_pid && WIFEXITED(exit_status))
			g->exit_val = WEXITSTATUS(exit_status);
		if (g_signal != 0)
			g->exit_val = 128 + g_signal;
		if (pid == -1)
			break ;
	}
}

int	dup_inf_out(t_global *g, int *std_save)
{
	if (g->cmds[std_save[2]]->infile != -1)
	{
		std_save[0] = dup(0);
		if (std_save[0] == -1 || dup2(g->cmds[std_save[2]]->infile, 0) == -1)
			return (handle_fd_error(g, std_save, true));
	}
	if (g->cmds[std_save[2]]->outfile != -1)
	{
		std_save[1] = dup(1);
		if (std_save[1] == -1 || dup2(g->cmds[std_save[2]]->outfile, 1) == -1)
			return (handle_out_error(g, std_save, true));
	}
	return (0);
}

int	exec_cmd(t_global *g, int id)
{
	int		std_save[3];

	if (set_check_cmd(g, 0))
		return (1);
	std_save[2] = id;
	if (dup_inf_out(g, std_save) == 1)
	{
		ft_perror("Exec: error with dup\n", 0);
		g->exit_val = 1;
		return (g->exit_val);
	}
	if (gest_builtins(g, g->cmds[id]) == NONE)
		gest_shell(g, id, std_save);
	close_fd(g, std_save, false);
	return (g->exit_val);
}
