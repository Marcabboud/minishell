/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir_parser.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eden <eden@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:24:48 by gmassin           #+#    #+#             */
/*   Updated: 2025/03/26 09:51:42 by eden             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	init_cmd_struct(t_cmd *cmd)
{
	cmd->args = NULL;
	cmd->redir = NULL;
	cmd->infile = -1;
	cmd->outfile = -1;
	cmd->prev_fd = -1;
	cmd->pipe[0] = -1;
	cmd->pipe[1] = -1;
}

static void	copy_inside_quotes(char *dest, char *line, int *i, int *j)
{
	char	quote_char;
	int		len;

	len = ft_strlen(line);
	while (*j < len)
	{
		if (line[*j] == '"' || line[*j] == '\'')
		{
			quote_char = line[*j];
			dest[(*i)++] = line[(*j)++];
			while (*j < len && line[*j] != quote_char)
				dest[(*i)++] = line[(*j)++];
			if (*j < len)
				dest[(*i)++] = line[(*j)++];
		}
		else if (!is_chars(" \t><", line[*j]))
			dest[(*i)++] = line[(*j)++];
		else
			break ;
	}
}

static int	parse_redirections(t_global *global_data, int cmd_index,
	char **line_array, int start_index)
{
	int		redir_index;
	int		pos;
	int		line_length;

	redir_index = 0;
	pos = start_index;
	line_length = ft_strlen(line_array[cmd_index]);
	while (pos < line_length)
	{
		if (is_chars("><", line_array[cmd_index][pos]))
		{
			while (pos < line_length && (is_chars("><",
						line_array[cmd_index][pos])
				|| is_chars(" \t", line_array[cmd_index][pos])))
				global_data->cmds[cmd_index]->redir[redir_index++]
					= line_array[cmd_index][pos++];
			copy_inside_quotes(global_data->cmds[cmd_index]->redir,
				line_array[cmd_index], &redir_index, &pos);
		}
		else
			pos++;
	}
	return (redir_index);
}

static int	extract_redirections(t_global *global_data, int cmd_index,
	char **line_array, int start_index)
{
	int	line_length;
	int	redir_index;

	line_length = ft_strlen(line_array[cmd_index]);
	if (start_index < line_length)
	{
		global_data->cmds[cmd_index]->redir = ft_calloc(sizeof(char),
				(line_length - start_index + 2));
		if (!global_data->cmds[cmd_index]->redir)
			return (ft_perror(ERR_ALLOCATION, 0), free_g(global_data,
					line_array), exit(1), -1);
		redir_index = parse_redirections(global_data, cmd_index,
				line_array, start_index);
		global_data->cmds[cmd_index]->redir[redir_index] = '\0';
		return (redir_index - 1);
	}
	return (start_index);
}

char	*build_cmd_base(t_global *global, int cmd_index,
			char **cmd_array, int indices[3])
{
	char	*cmd_str;
	int		len;

	len = ft_strlen(cmd_array[cmd_index]);
	cmd_str = ft_calloc(sizeof(char), (len + 1));
	if (!cmd_str)
		return (ft_perror(ERR_ALLOCATION, 0), free_g(global, cmd_array),
			exit(1), (char *)0);
	while (cmd_array[cmd_index][++indices[0]])
		if (is_chars("><", cmd_array[cmd_index][indices[0]])
			&& !inq(cmd_array[cmd_index], indices[0], 0))
			break ;
	init_cmd_struct(global->cmds[cmd_index]);
	indices[2] = extract_redirections(global, cmd_index, cmd_array, indices[0]);
	ft_strncpy(cmd_str, cmd_array[cmd_index], indices[0]);
	indices[1] = indices[0];
	return (cmd_str);
}
