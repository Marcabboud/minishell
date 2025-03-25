#include "../../inc/minishell.h"
int	ft_exit(t_global *g, t_cmd *cmd, bool print)
{
	if (print)
        	printf("exit\n");
    	if (!cmd->args[1])
    {
	free_g(g, NULL);
        exit(g->exit_val);
    }
    	return (ft_exit_validate(g, cmd));
}
int	ft_exit_validate(t_global *g, t_cmd *cmd)
{
	int nb;

    	if ((	ft_is_nb(cmd->args[1]) || 	ft_strlen(cmd->args[1]) >= 19) &&
        ((ft_strncmp("9223372036854775807", cmd->args[1], 19) < 0 && cmd->args[1][0] != '-') ||
         (ft_strncmp("-9223372036854775808", cmd->args[1], 20) < 0 && cmd->args[1][0] == '-')))
    {
	ft_perror("exit: ", 0);
        	ft_perror(cmd->args[1], 0);
        	ft_perror(": numeric argument required\n", 0);
        	free_g(g, NULL);
        exit(2);
    }
    	if (	ft_is_nb(cmd->args[1]) == 0 && !cmd->args[2])
    {
	nb = 	ft_atoi(cmd->args[1]);
        	free_g(g, NULL);
        exit(nb % 256);
    }
    	ft_perror("exit: too many arguments\n", 0);
    	g->exit_val = 1;
    	return (1);
}
static	t_state	ft_echo(t_global *g, t_cmd *cmd)
{
	int	i = 1;
    bool	new_line = true;

    	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-n"))
    {
	new_line = false;
        i++;
    }
    while (cmd->args[i])
    {
	write(1, cmd->args[i], 	ft_strlen(cmd->args[i]));
        	if (cmd->args[++i])
            	write(1, " ", 1);
    }
    	if (new_line)
        	write(1, "\n", 1);
    	g->exit_val = 0;
    	return (VALID);
}
static	t_state	gest_env(t_global *g, char **arr)
{
	char	*result;

    	if (ft_strcmp(arr[0], "env"))
    {
	if (arr[1])
        {
	ft_perror("env: '", 0);
            	ft_perror(arr[1], 0);
            	ft_perror("': No such file or directory\n", 0);
            	g->exit_val = 1;
            	return (ERROR);
        }
        	return (	plist(g->lenv, NULL), VALID);
    }
    	if (	ft_strcmp(arr[0], "pwd") && (result = 	plist(g->lenv, "PWD")))
    {
	printf("%s\n", result);
        free(result);
        	return (VALID);
    }
    	if (	ft_strcmp(arr[0], "unset"))
        	return (	ft_unset(g, &arr[1]), VALID);
    	if (	ft_strcmp(arr[0], "cd"))
        	return (	ft_cd(g, arr), VALID);
    	g->exit_val = 1;
    	return (ERROR);
}

