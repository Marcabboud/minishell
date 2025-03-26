/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eden <eden@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:21:55 by gmassin           #+#    #+#             */
/*   Updated: 2025/03/26 09:47:55 by eden             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	append_cmd_suffix(char *cmd_str, int cmd_index, char **cmd_array,
			int indices[3])
{
	int	len;

	len = ft_strlen(cmd_array[cmd_index]);
	while (indices[0] < len && cmd_array[cmd_index][indices[0]])
	{
		if (indices[0] < indices[2])
			indices[0] = indices[2];
		if (cmd_array[cmd_index][indices[0]] == ' ' &&
			!is_chars(">< \t", cmd_array[cmd_index][indices[0] - 1]))
			while (cmd_array[cmd_index][indices[0]] &&
				!is_chars("><", cmd_array[cmd_index][indices[0]]))
				cmd_str[indices[1]++] = cmd_array[cmd_index][indices[0]++];
		indices[0]++;
	}
	cmd_str[indices[1]] = '\0';
}

static void	build_full_cmd(t_global *global, int cmd_index, char **cmd_array,
			int indices[3])
{
	char	*cmd_line;
	char	*expanded_line;

	cmd_line = build_cmd_base(global, cmd_index, cmd_array, indices);
	append_cmd_suffix(cmd_line, cmd_index, cmd_array, indices);
	expanded_line = expand_env(global, cmd_line);
	global->cmds[cmd_index]->args = str_to_arr(expanded_line, " \t", true);
	free(cmd_line);
	free(expanded_line);
}

static void	build_all_cmds(t_global *global_data, char **cmd_array)
{
	int		i;
	int		indices[3];

	i = 0;
	while (i < global_data->cnt)
	{
		global_data->cmds[i] = ft_calloc(sizeof(t_cmd), 1);
		if (!global_data->cmds[i])
			return (ft_perror(ERR_ALLOCATION, 0), (void)0);
		indices[0] = -1;
		build_full_cmd(global_data, i, cmd_array, indices);
		if (!global_data->cmds[i]->args)
			return (ft_perror(ERR_ALLOCATION, 0),
				free_g(global_data, cmd_array), exit(1), (void)0);
		i++;
	}
	global_data->cmds[i] = NULL;
}

void	init_cmd_structs(t_global *global_data, char *input_line)
{
	char	**cmd_array;

	global_data->cnt = count_rows("|", input_line, true);
	global_data->cmds = ft_calloc(sizeof(t_cmd *), (global_data->cnt + 1));
	if (!global_data->cmds)
		return (ft_perror(ERR_ALLOCATION, 0), free(input_line), exit(1),
			(void)0);
	cmd_array = str_to_arr(input_line, "|", true);
	if (!cmd_array)
		return (ft_perror(ERR_ALLOCATION, 0), free_g(global_data, 0),
			free(input_line), exit(1), (void)0);
	build_all_cmds(global_data, cmd_array);
	free_arr(cmd_array);
}
