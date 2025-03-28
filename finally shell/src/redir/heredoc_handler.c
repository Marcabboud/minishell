/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:21:06 by gmassin           #+#    #+#             */
/*   Updated: 2025/03/27 16:42:06 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*init_heredoc_buffer(char *line, t_env *lenv, int i,
			size_t total_size)
{
	char	*var_name;
	char	*var_val;

	while (i < ft_strlen(line) && line[i])
	{
		if (line[i] == '$')
		{
			i = hdoc_extract_var(line, ++i, &var_name, true);
			if (i == -1)
				return (NULL);
			var_val = get_env_content(lenv, var_name);
			free(var_name);
			if (var_val)
			{
				total_size += ft_strlen(var_val);
				free(var_val);
			}
			else
				total_size++;
		}
		else
			total_size++;
		i++;
	}
	return (ft_calloc(total_size + 1, sizeof(char)));
}

static char	*fill_heredoc_buffer(char *line, t_env *lenv, char *hd, int i)
{
	char	*var_name;
	char	*var_val;
	int		pos;

	pos = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			i = hdoc_extract_var(line, ++i, &var_name, false);
			if (i == -1)
				return (NULL);
			var_val = get_env_content(lenv, var_name);
			free(var_name);
			if (!var_val)
				continue ;
			ft_strlcat(hd + pos, var_val, ft_strlen(var_val) + 1);
			pos += ft_strlen(var_val);
			free(var_val);
		}
		else
			hd[pos++] = line[i++];
	}
	hd[pos] = 0;
	return (hd);
}

static char	*parse_heredoc_line(char *line, t_env *lenv)
{
	char	*here_doc;

	here_doc = init_heredoc_buffer(line, lenv, 0, 0);
	if (!here_doc)
		return (ft_perror(ERR_ALLOC, 0), NULL);
	here_doc = fill_heredoc_buffer(line, lenv, here_doc, 0);
	if (!here_doc)
		return (line);
	return (here_doc);
}

int	heredoc_quoted(t_cmd *cmd, int *fd, char *del, void (*old_handler)(int))
{
	char	*inpt_line;

	while (g_signal != SIGINT)
	{
		inpt_line = readline("minishell heredoc(not parsed)> ");
		if (!inpt_line || g_signal == SIGINT)
		{
			if (g_signal == SIGINT)
			{
				(close(fd[0]), fd[0] = -2);
				break ;
			}
			ft_perror(ERR_HD_EOF, 0);
			break ;
		}
		if ((ft_strncmp(inpt_line, del, ft_strlen(del)) == 0
				&& inpt_line[ft_strlen(del)] == 0))
			break ;
		(hdoc_write(inpt_line, fd[1], ft_strlen(inpt_line)), free(inpt_line));
	}
	(signal(SIGINT, old_handler), free(inpt_line), close(fd[1]));
	cmd->infile = fd[0];
	if (fd[0] == -2)
		return (-1);
	return (0);
}

int	heredoc_unquoted(t_global *g, int *fd, char *del, void (*old_handler)(int))
{
	char	*inpt_line;

	while (g_signal != SIGINT)
	{
		inpt_line = readline("minishell heredoc> ");
		if (g_signal == SIGINT)
		{
			(close(fd[0]), fd[0] = -2);
			break ;
		}
		if (!inpt_line)
		{
			ft_perror(ERR_HD_EOF, 0);
			break ;
		}
		if ((ft_strncmp(inpt_line, del, ft_strlen(del)) == 0
				&& inpt_line[ft_strlen(del)] == 0))
			break ;
		inpt_line = parse_heredoc_line(inpt_line, g->lenv);
		(hdoc_write(inpt_line, fd[1], ft_strlen(inpt_line)), free(inpt_line));
	}
	(signal(SIGINT, old_handler), free(inpt_line), close(fd[1]));
	if (fd[0] == -2)
		return (g->cmds[fd[2]]->infile = fd[0], -1);
	return (g->cmds[fd[2]]->infile = fd[0], 0);
}
