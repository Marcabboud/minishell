/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:20:57 by gmassin           #+#    #+#             */
/*   Updated: 2025/03/27 16:09:03 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*pwrapper(char *name, char *content, char sep)
{
	char	*result;
	int		total_length;
	int		i;

	total_length = ft_strlen(name) + ft_strlen(content) + 1;
	i = 0;
	result = ft_calloc(sizeof(char), (total_length + 1));
	if (!result)
		return (NULL);
	while (*name)
		result[i++] = *name++;
	result[i++] = sep;
	while (*content)
		result[i++] = *content++;
	result[i] = '\0';
	return (result);
}

int	skip_charset(char *str, char *dels)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] && (is_chars(dels, str[i])))
		i++;
	return (i);
}

char	*get_env_content(t_env *lenv, char *name)
{
	t_env	*current;
	char	*result;

	current = lenv;
	while (current != NULL)
	{
		if (name && ft_strcmp(name, current->name))
		{
			result = ft_strdup(current->content);
			return (result);
		}
		else if (!name)
		{
			result = remove_quotes(ft_strdup(current->content));
			printf("%s=%s\n", current->name, result);
			free(result);
		}
		current = current->next;
	}
	return (NULL);
}

bool	inq(char *str, int index, char quote)
{
	int		i;
	char	active_quote;

	i = 0;
	active_quote = '\0';
	if (!str || index < 0 || index >= (int)ft_strlen(str))
		return (false);
	while (i <= index)
	{
		if (active_quote == '\0')
		{
			if ((quote == '\0' && (str[i] == 34 || str[i] == 39))
				|| str[i] == quote)
				active_quote = str[i];
		}
		else if (str[i] == active_quote)
			active_quote = '\0';
		i++;
	}
	return (active_quote != '\0');
}

int	syntax_error_msg(char *str, int pos)
{
	if (str[pos] && str[pos] == '>' && str[pos + 1] && str[pos + 1] == '>')
		return (ft_perror(UNEXPECTED_TOKEN, ">>"), -1);
	else if (str[pos] && str[pos] == '>')
		return (ft_perror(UNEXPECTED_TOKEN, ">"), -1);
	else if (str[pos] && str[pos] == '<' && str[pos + 1] && str[pos + 1] == '<')
		return (ft_perror(UNEXPECTED_TOKEN, "<<"), -1);
	else if (str[pos] && str[pos] == '<')
		return (ft_perror(UNEXPECTED_TOKEN, "<"), -1);
	else if (str[pos] && str[pos] == '|' && str[pos + 1] && str[pos + 1] != '|')
		return (ft_perror(UNEXPECTED_TOKEN, "|"), -1);
	else if (str[pos] && str[pos] == '|')
		return (ft_perror(UNEXPECTED_TOKEN, "|"), -1);
	return (-1);
}
