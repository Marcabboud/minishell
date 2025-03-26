/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_builder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eden <eden@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:22:34 by gmassin           #+#    #+#             */
/*   Updated: 2025/03/26 09:51:42 by eden             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	is_chars(const char *chr, char c)
{
	while (*chr)
	{
		if (c == *chr)
			return (true);
		chr++;
	}
	return (false);
}

int	count_rows(const char *chr, char *str, bool quote)
{
	int	rows_count;
	int	i;

	rows_count = 0;
	i = 0;
	while (str[i] && is_chars(chr, str[i]))
		i++;
	while (str[i])
	{
		if ((!quote && str[i + 1] && is_chars(chr, str[i])
				&& !is_chars(chr, str[i + 1])) || (quote && str[i + 1]
				&& is_chars(chr, str[i]) && !is_chars(chr, str[i + 1])
				&& !inq(str, i, 0)))
			rows_count++;
		i++;
	}
	return (rows_count + 1);
}

static int	*count_cols(const char *chr, char *str, int rows, bool quote)
{
	int	*cols;
	int	counter[2];
	int	i;

	cols = malloc(sizeof(int) * (rows + 1));
	if (!cols)
		return (0);
	counter[0] = 0;
	counter[1] = 0;
	i = 0;
	while (str[i])
	{
		if ((!is_chars(chr, str[i]) && str[i])
			|| (quote && is_chars(chr, str[i]) && inq(str, i, 0)))
			counter[0]++;
		if ((!quote && counter[0] > 0 && is_chars(chr, str[i])) || (quote
				&& counter[0] > 0 && is_chars(chr, str[i]) && !inq(str, i, 0)))
		{
			cols[counter[1]++] = counter[0];
			counter[0] = 0;
		}
		i++;
	}
	cols[counter[1]] = counter[0];
	return (cols);
}

static char	**init_arr(const char *chr, char *str, bool quote)
{
	char	**arr;
	int		*cols;
	int		total_rows;
	int		row;

	total_rows = count_rows(chr, str, quote);
	cols = count_cols(chr, str, total_rows, quote);
	if (!cols)
		return (NULL);
	arr = malloc(sizeof(char *) * (total_rows + 1));
	if (!arr)
		return (free(cols), NULL);
	row = 0;
	while (row < total_rows)
	{
		arr[row] = ft_calloc(sizeof(char), (cols[row] + 1));
		if (!arr[row])
			return (free_arr(arr), free(cols), NULL);
		row++;
	}
	arr[row] = NULL;
	return (free(cols), arr);
}

char	**str_to_arr(char *str, const char *chr, bool quote)
{
	char	**arr;
	int		row;
	int		col;
	int		i;

	arr = init_arr(chr, str, quote);
	if (!arr)
		return (NULL);
	col = 0;
	row = 0;
	i = 0;
	while (str[i])
	{
		if ((!is_chars(chr, str[i]) && str[i])
			|| (quote && is_chars(chr, str[i]) && inq(str, i, 0)))
			arr[row][col++] = str[i];
		if ((!quote && col > 0 && is_chars(chr, str[i]))
			|| (quote && col > 0 && is_chars(chr, str[i]) && !inq(str, i, 0)))
		{
			arr[row++][col] = '\0';
			col = 0;
		}
		i++;
	}
	return (arr);
}
