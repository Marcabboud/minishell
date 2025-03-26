/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eden <eden@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:22:22 by gmassin           #+#    #+#             */
/*   Updated: 2025/03/26 09:51:42 by eden             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	copy_inside_quotes(char *dest, char *src, int *i, int *j)
{
	char	quote_char;

	quote_char = src[*i];
	(*i)++;
	while (src[*i] && src[*i] != quote_char)
		dest[(*j)++] = src[(*i)++];
	if (src[*i] == quote_char)
		(*i)++;
}

char	*remove_quotes(char *str)
{
	char	*result;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(str);
	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			copy_inside_quotes(result, str, &i, &j);
			continue ;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (free(str), result);
}

char	*get_env_val(t_env *lenv, char *src, int *i)
{
	char	*var_name;
	char	*var_value;
	int		len;

	len = 0;
	while (src[len] && (ft_isalnum(src[len]) || src[len] == '_'))
		len++;
	if (len == 0)
		return (NULL);
	*i += len;
	var_name = malloc(sizeof(char) * (len + 1));
	if (!var_name)
		return (ft_perror(ERR_ALLOCATION, 0), NULL);
	ft_strncpy(var_name, src, len);
	var_value = get_env_content(lenv, var_name);
	free(var_name);
	if (!var_value)
		return (NULL);
	return (var_value);
}

static char	*expand_dollar_variable(t_global *g, char *str, int *i, char *buf)
{
	char	*value;
	int		y;

	y = 0;
	(*i)++;
	if (str[*i] == '\0' || is_chars(" \t\"", str[*i]))
	{
		buf[y++] = '$';
		return (buf + y);
	}
	if (str[*i] == '?')
	{
		value = ft_itoa(g->exit_val);
		while (*value)
			buf[y++] = *value++;
		(*i)++;
		return (free(value - y), buf + y);
	}
	value = get_env_val(g->lenv, &str[*i], i);
	if (value)
	{
		while (*value)
			buf[y++] = *value++;
	}
	return (free(value - y), buf + y);
}

char	*expand_env(t_global *g, char *str)
{
	char	*buf;
	char	*buf_pos;
	int		i;

	buf = ft_calloc(sizeof(char), BUFSIZ);
	if (!buf)
		return (ft_perror(ERR_ALLOCATION, 0), NULL);
	buf_pos = buf;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$' && !inq(str, i, '\''))
			buf_pos = expand_dollar_variable(g, str, &i, buf_pos);
		else
		{
			*buf_pos = str[i];
			buf_pos++;
			i++;
		}
	}
	*buf_pos = '\0';
	return (buf);
}
