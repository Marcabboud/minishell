/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabboud <mabboud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:22:48 by gmassin           #+#    #+#             */
/*   Updated: 2025/03/28 13:31:04 by mabboud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <fcntl.h>
# include <errno.h>
# include <stdarg.h>
# include <signal.h>
# include <sys/wait.h>
# include "struct.h"
# define _POSIX_C_SOURCE 200809L

extern int	g_signal;

/*LIB*/
int		ft_strlen(const char *str);
bool	ft_strcmp(char *s1, char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *src);
void	ft_strncpy(char *dst, char *src, int n);
size_t	ft_strlcat(char *dest, const char *src, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
int		ft_isalnum(int c);
void	*ft_calloc(size_t nmemb, size_t size);
void	*ft_memset(void *p, int i, size_t len);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
int		ft_isdigit(int c);
int		ft_atoi(const char *str);
int		ft_is_nb(char *cmd);
char	*ft_itoa(int n);
void	ft_perror(const char *str, char *tok);
char	*ft_substr(char const *s, unsigned int start, size_t len);
void	ft_free_node(t_env *node);

/*BUILTINS*/
t_state	dispatch_cmd(t_global *g, t_cmd *cmd);
int		ft_exit(t_global *g, t_cmd *cmd, bool message);
bool	ft_export(t_global *g, char **str, bool multiples);
bool	ft_cd(t_global *g, char **arr);
void	ft_unset(t_global *g, char **str);
t_state	format_export(char *str);

/*SIGNAL*/
void	exit_shell(t_global *g);
void	heredoc_signal_handler(int sig);

/*EXEC*/
void	exec_cmd(t_global *g, int id);
int		prep_cmd_exec(t_global *g, int id);
int		execute_cmd(t_global *g, int id);
void	close_child_fds(t_global *g);
int		ft_exec(t_global *g);
void	exec_pipes(t_global *g);
void	ft_wait_for_childs(t_global *g);

/*PARSING*/
bool	syntax_is_valid(t_global *g, char *str);
t_env	*create_node(char *str);
t_env	*env_arr_to_list(char **env);
char	**env_list_to_arr(t_env *lenv);
char	**str_to_arr(char *str, const char *chr, bool quote);
void	init_cmd_structs(t_global *g, char *line);
char	*expand_env(t_global *g, char *str);
int		syntax_error_msg(char *str, int idx);
int		check_pipe_syntax(char *str);
bool	check_double_pipe(const char *str);

/*UTILS*/
bool	inq(char *str, int index, char quote);
char	*remove_quotes(char *str);
bool	is_chars(const char *chr, char c);
char	*pwrapper(char *name, char *content, char sep);
int		count_rows(const char *chr, char *str, bool quote);
int		skip_charset(char *str, char *dels);
int		check_quotes(char *str, int i);
char	*get_env_content(t_env *lenv, char *name);

/*FREE*/
void	free_arr(char **arr);
void	free_list(t_env *list);
void	free_cmds(t_global *g);
void	free_cmd(t_cmd *cmd);
void	free_g(t_global *g, char **arr);

/*REDIR*/
int		redir_process(t_global *g, int i, int tmp);
int		redir_overwrite(t_global *g, t_cmd *cmd, char *redir);
int		redir_append(t_global *g, t_cmd *cmd, char *redir);
int		redir_input(t_global *g, t_cmd *cmd, char *redir);
int		redir_heredoc(t_global *g, int id, char *redir);
int		heredoc_unquoted(t_global *g, int *fd, char *del,
			void (*old_handler)(int));
int		heredoc_quoted(t_cmd *cmd, int *fd, char *del,
			void (*old_handler)(int));
ssize_t	hdoc_write(const void *buffer, int fd, size_t count);
int		hdoc_extract_var(char *line, int len, char **v_name, bool del_sign);
char	*redir_filename(char *redir, int i, int len, char *dels);

char	*build_cmd_base(t_global *global, int cmd_index,
			char **cmd_array, int indices[3]);

#endif
