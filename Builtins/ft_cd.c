#include "../../inc/minishell.h"

static bool	update_pwd(t_global *g, char *pl, char *tmp)
{
	tmp = pwrapper("OLDPWD", pl, '=');
	free(pl);
	ft_export(g, &tmp, false);
	free(tmp);
	return (true);
}

static bool	update_cwd(t_global *g, char *cwd)
{
	char	*tmp;

	tmp = pwrapper("PWD", cwd, '=');
	ft_export(g, &tmp, false);
	free(tmp);
	return (true);
}

static bool	simple_cd(t_global *g, char *pl)
{
	char	*tmp;
	*tmp = NULL;

	pl = plist(g->lenv, "HOME");
	if (!pl)
	{
		ft_perror("cd: HOME not set\n", 0);
		g->exit_val = 1;
		return (false);
	}
	chdir(pl);
	update_pwd(g, plist(g->lenv, "PWD"), tmp);
	update_cwd(g, pl);
	free(pl);
	return (true);
}

bool    ft_cd(t_global *g, char **arr)                                          
{                                                                               
    char    cwd[1024];                                                          
    char    *pl;                                                                
                                                                                
    if (!arr[1] || ft_strcmp(arr[1], "~"))                                      
        return (simple_cd(g, NULL));                                            
    if (arr[2])                                                                 
        return (g->exit_val = 1, ft_perror(" too many arguments\n", 0), false);  
    if (chdir(arr[1]) == -1)                                                    
        return (g->exit_val = 1, perror("cd"), false);                           
    if (getcwd(cwd, sizeof(cwd)) != NULL)                                       
    {                                                                           
        pl = plist(g->lenv, "PWD");                                             
        update_pwd(g, pl, NULL);                                                
        update_cwd(g, cwd);                                                     
        return (true);                                                          
    }                                                                           
    perror("getcwd");                                                           
    return (false);                                                             
}

