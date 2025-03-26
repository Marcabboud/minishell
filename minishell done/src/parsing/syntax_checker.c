/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eden <eden@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:22:27 by gmassin           #+#    #+#             */
/*   Updated: 2025/03/26 09:52:00 by eden             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_pipe_syntax(char *str)
{
	int	pos;

	if (*str != '|')
		return (0);
	pos = 1;
	pos += skip_charset(str, " \t");
	if (!str[pos] || (str[pos] == '|' && str[pos - 1] != '|'))
		return (ft_perror(UNEXPECTED_TOKEN, "|"), -1);
	if (str[pos] == '|' && str[pos - 1] == '|')
		pos++;
	return (pos);
}

static int	check_redir_syntax(char *str)
{
	int	pos;

	if (!*str || !is_chars("><", *str))
		return (0);
	pos = 0;
	if (ft_strncmp(str + pos, ">>", 2) == 0)
		pos += 2;
	else if (str[pos] == '>')
		pos++;
	else if (ft_strncmp(str + pos, "<<", 2) == 0)
		pos += 2;
	else if (str[pos] == '<')
		pos++;
	while (str[pos] && is_chars("\t ", str[pos]))
		pos++;
	if (!str[pos] || pos == ft_strlen(str))
		return (ft_perror(UNEXPECTED_TOKEN, "newline"), -1);
	if (is_chars("><|", str[pos]))
		return (syntax_error_msg(str, pos), -1);
	return (pos);
}

int	check_quotes(char *str, int i)
{
	char	open_quote;
	int		pos;

	open_quote = 0;
	if (str[0] && ft_strlen(str) == 1 && is_chars("\'\"", str[0]))
		return (ft_perror("Error: Unmatched quote\n", 0), (int)-1);
	pos = 0;
	while (str[pos])
		if (is_chars("\'\"", str[pos++]))
			break ;
	if (pos == ft_strlen(str))
		return (i);
	pos = i;
	while (str[pos])
	{
		if (open_quote == 0 && (str[pos] == 34 || str[pos] == 39))
			open_quote = str[pos];
		else if (open_quote != 0 && str[pos] == open_quote)
			open_quote = 0;
		pos++;
	}
	if (open_quote != 0)
		return (ft_perror("Error: Unmatched quote\n", 0), (int)-1);
	return (pos);
}

static int	get_next_token(char *str, int i)
{
	int	temp;

	while (str[i] && !is_chars("><|\"'", str[i]))
		i++;
	temp = check_quotes(str, i);
	if (temp < 0)
		return (-1);
	i += temp - i;
	temp = check_pipe_syntax(str + i);
	if (temp < 0)
		return (-1);
	i += temp;
	temp = check_redir_syntax(str + i);
	if (temp < 0)
		return (-1);
	i += temp;
	if (i == 0)
		return (-1);
	return (i);
}

bool	syntax_is_valid(t_global *g, char *str)
{
	int	i;
	int	temp;

	add_history(str);
	if (g_signal == SIGINT)
		g->exit_val = 130;
	if (str == NULL)
		signal_ctrd(g);
	i = skip_charset(str, " \t");
	if (!str[i])
		return (false);
	if (str[i] == '|')
		return (ft_perror(UNEXPECTED_TOKEN, "|"), g->exit_val = 2, false);
	while (str[i])
	{
		temp = get_next_token(str, i);
		if (temp < 0)
			return (g->exit_val = 2, false);
		i += temp;
		if (i > (int)ft_strlen(str))
			break ;
	}
	return (true);
}
