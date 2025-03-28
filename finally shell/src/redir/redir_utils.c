/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:21:33 by gmassin           #+#    #+#             */
/*   Updated: 2025/03/27 16:09:12 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	get_redir_filename_len(char *redir, char *dels)
{
	int		index;
	char	quote;
	int		actual_length;

	index = skip_charset(redir, " \t");
	quote = '\0';
	actual_length = 0;
	while (redir[index] && (!is_chars(dels, redir[index])
			|| (is_chars("'\"", quote) && redir[index] != quote)))
	{
		if (is_chars("'\"", redir[index]) && !quote)
			quote = redir[index];
		else if (redir[index] == quote)
			quote = '\0';
		else
			actual_length++;
		index++;
	}
	return (actual_length);
}

char	*redir_filename(char *redir, int i, int len, char *dels)
{
	char	*file;
	char	quote;

	file = ft_calloc(sizeof(char), (get_redir_filename_len(redir, dels) + 1));
	if (!file)
		return (NULL);
	i = skip_charset(redir, " \t") - 1;
	quote = '\0';
	while (redir[++i] && (!is_chars(dels, redir[i])
			|| (is_chars("'\"", quote) && redir[i] != quote)))
	{
		if ((is_chars("'\"", redir[i]) && !quote) || redir[i] == quote)
		{
			if (redir[i] == quote)
				quote = '\0';
			else
				quote = redir[i];
			continue ;
		}
		file[len++] = redir[i];
	}
	if (!quote)
		if (len > 0 && file[len - 1] == ' ')
			len--;
	return (file[len] = '\0', file);
}

void	heredoc_signal_handler(int sig)
{
	g_signal = sig;
	close(STDIN_FILENO);
}
