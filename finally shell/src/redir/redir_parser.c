/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:21:15 by gmassin           #+#    #+#             */
/*   Updated: 2025/03/27 16:09:10 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

ssize_t	hdoc_write(const void *buffer, int fd, size_t count)
{
	const char	*buf;
	size_t		total_written;
	ssize_t		bytes_written;

	buf = buffer;
	total_written = 0;
	while (total_written < count)
	{
		bytes_written = write(fd, buf + total_written, count - total_written);
		if (bytes_written > 0)
			total_written += bytes_written;
		else if (bytes_written == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				continue ;
			else
				return (-1);
		}
	}
	write(fd, "\n", 1);
	return (total_written);
}

int	hdoc_extract_var(char *line, int len, char **var_name, bool del_sign)
{
	int	start_pos;
	int	var_length;

	start_pos = len;
	while (line[len] && (ft_isalnum(line[len]) || line[len] == '_'))
		len++;
	var_length = len - start_pos;
	if (line[len] && del_sign)
		len--;
	*var_name = ft_calloc(sizeof(char), (var_length + 1));
	if (!*var_name)
		return (-1);
	ft_strncpy(*var_name, line + start_pos, var_length);
	(*var_name)[var_length] = '\0';
	return (len);
}

static int	skip_non_redir_chars(char *redir, int i)
{
	int	skip_count;
	int	redir_length;

	skip_count = 0;
	if (!redir)
		return (0);
	redir_length = ft_strlen(redir);
	if (i > redir_length || skip_count > redir_length
		|| i + skip_count > redir_length)
		return (0);
	while (i + skip_count < ft_strlen(redir)
		&& !is_chars("><|", redir[i + skip_count]))
	{
		if (!is_chars("'\"", redir[i + skip_count]))
			skip_count++;
		if (i + skip_count < ft_strlen(redir))
		{
			skip_count += skip_charset(redir + i + skip_count, "\"");
			skip_count += skip_charset(redir + i + skip_count, "'");
		}
	}
	return (skip_count);
}

static int	skip_until_pipe(char *redir)
{
	int	pos;

	pos = 0;
	while (redir[pos] && redir[pos] != '|')
		pos++;
	return (pos);
}

int	redir_process(t_global *g, int i, int tmp)
{
	int	j;

	while (++i < g->cnt && g->cmds[i])
	{
		if (!g->cmds[i]->redir)
			continue ;
		j = 0;
		while (j < ft_strlen(g->cmds[i]->redir) && g->cmds[i]->redir[j])
		{
			if (ft_strncmp(g->cmds[i]->redir + j, ">>", 2) == 0)
				tmp = redir_append(g, g->cmds[i], g->cmds[i]->redir + j + 2);
			else if (g->cmds[i]->redir[j] == '>')
				tmp = redir_overwrite(g, g->cmds[i], g->cmds[i]->redir + j + 1);
			if (ft_strncmp(g->cmds[i]->redir + j, "<<", 2) == 0)
				tmp = redir_heredoc(g, i, g->cmds[i]->redir + j + 2);
			else if (g->cmds[i]->redir[j] == '<')
				tmp = redir_input(g, g->cmds[i], g->cmds[i]->redir + j + 1);
			if (tmp < 0)
				j += skip_until_pipe(g->cmds[i]->redir + j);
			if (!g->cmds[i]->redir[j])
				break ;
			j += tmp + skip_non_redir_chars(g->cmds[i]->redir, (j + tmp));
		}
	}
	return (1);
}
