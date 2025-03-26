/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eden <eden@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:21:25 by gmassin           #+#    #+#             */
/*   Updated: 2025/03/26 09:51:42 by eden             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	redir_overwrite(t_global *g, t_cmd *cmd, char *redir)
{
	t_cmd	*local_cmd;
	char	*file_name;

	local_cmd = cmd;
	if (local_cmd->outfile > 1)
		close(local_cmd->outfile);
	redir = redir + skip_charset(redir, " \t");
	file_name = redir_filename(redir, 0, 0, "<>| ");
	if (!file_name)
		return (-1);
	cmd->outfile = open(file_name, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (cmd->outfile == -1)
	{
		cmd->outfile = -2;
		g->exit_val = 1;
		(perror(file_name), free(file_name));
		return (-1);
	}
	free(file_name);
	return (1);
}

int	redir_append(t_global *g, t_cmd *cmd, char *redir)
{
	t_cmd	*local_cmd;
	char	*file_name;

	local_cmd = cmd;
	if (local_cmd->outfile > 1)
		close(local_cmd->outfile);
	redir = redir + skip_charset(redir, " \t");
	file_name = redir_filename(redir, 0, 0, "<>| ");
	if (!file_name)
		return (-1);
	cmd->outfile = open(file_name, O_CREAT | O_APPEND | O_RDWR, 0666);
	if (cmd->outfile == -1)
	{
		cmd->outfile = -2;
		g->exit_val = 1;
		(perror(file_name), free(file_name));
		return (-1);
	}
	free(file_name);
	return (2);
}

int	redir_input(t_global *g, t_cmd *cmd, char *redir)
{
	t_cmd	*local_cmd;
	char	*file_name;

	local_cmd = cmd;
	if (local_cmd->infile > 1)
		close(local_cmd->infile);
	redir = redir + skip_charset(redir, " \t");
	file_name = redir_filename(redir, 0, 0, "<>| ");
	if (!file_name)
		return (-1);
	cmd->infile = open(file_name, O_RDONLY);
	if (cmd->infile == -1)
	{
		cmd->infile = -2;
		g->exit_val = 1;
		(perror(file_name), free(file_name));
		return (-1);
	}
	free(file_name);
	return (1);
}

static int	execute_heredoc_child(t_global *g, int id, int *fd, char *redir)
{
	int		pos;
	void	(*prev_handler)(int);
	char	*file_name;
	int		result;

	prev_handler = signal(SIGINT, heredoc_signal_handler);
	pos = skip_charset(redir, " \t");
	fd[2] = id;
	while (is_chars("<>|", redir[pos]))
		pos++;
	signal(SIGINT, heredoc_signal_handler);
	if (inq(redir, pos, '\0'))
	{
		file_name = redir_filename(redir + pos, 0, 0, "<>|");
		if (!file_name)
			return (close(fd[1]), close(fd[0]), -1);
		result = pos + heredoc_quoted(g->cmds[id], fd, file_name, prev_handler);
		return (free(file_name), result);
	}
	file_name = redir_filename(redir + pos, 0, 0, "<>| ");
	if (!file_name)
		return (close(fd[1]), close(fd[0]), -1);
	result = pos + heredoc_unquoted(g, fd, file_name, prev_handler);
	return (free(file_name), result);
}

int	redir_heredoc(t_global *g, int id, char *redir)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	status = 0;
	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (close(fd[0]), close(fd[1]), -1);
	if (pid == 0)
	{
		close(fd[0]);
		status = execute_heredoc_child(g, id, fd, redir);
		(free_g(g, NULL), exit(status));
	}
	(close(fd[1]), waitpid(pid, &status, 0));
	if (WIFSIGNALED(status))
	{
		close(fd[0]);
		g->cmds[id]->infile = -2;
		return (-1);
	}
	g->cmds[id]->infile = fd[0];
	return (2);
}
