/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:22:42 by gmassin           #+#    #+#             */
/*   Updated: 2025/03/27 16:08:47 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include <sys/wait.h>
# define SINGLE_QUOTE 39
# define DOUBLE_QUOTE 34
# define ERR_ALLOCATION "Error: allocation failed.\n"
# define UNEXPECTED_TOKEN "syntax error near unexpected token "
# define INVALID_IDENTIFIER "not a valid identifier\n"
# define ERR_HD_EOF "warning: here-document delimited by end-of-file\n"
# define ERR_ALLOC "An error has occured, it may\
 be related with a malloc failure\n"

typedef enum state
{
	NONE,
	VALID,
	ERROR,
}	t_state;

typedef struct s_env
{
	char			*name;
	char			*content;
	struct s_env	*next;
}	t_env;

typedef struct s_cmd
{
	char	**args;
	char	*redir;
	int		pipe[2];
	int		infile;
	int		outfile;
	int		prev_fd;
}	t_cmd;

typedef struct s_global
{
	t_cmd		**cmds;
	t_env		*lenv;
	pid_t		last_pid;
	int			exit_val;
	int			cnt;
}	t_global;

#endif
