/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 15:53:37 by gmassin           #+#    #+#             */
/*   Updated: 2025/03/27 16:09:42 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	check_double_pipe(const char *str)
{
	int	i;
	int	j;
	int	in_single;
	int	in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '\"' && !in_single)
			in_double = !in_double;
		else if (!in_single && !in_double && str[i] == '|')
		{
			j = i + 1;
			while (str[j] && (str[j] == ' ' || str[j] == '\t'))
				j++;
			if (str[j] == '|')
				return (ft_perror(UNEXPECTED_TOKEN, "|"), false);
		}
		i++;
	}
	return (true);
}

int	check_pipe_syntax(char *str)
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
