#include "../minishell.h"

void	init_cmd(t_cmd *cmd)
{
	cmd->readfd = -1;
	cmd->writefd = -1;
    cmd->count = 0;
	cmd->pp[0] = -1;
	cmd->pp[1] = -1;
	cmd->pathname = NULL;
	cmd->cmd = NULL;
	cmd->err_msg = NULL;
}

void    safe_pipe(t_cmd *cmd)
{
    if (pipe(cmd->pp) == -1)
        print_error_and_exit(strerror(errno));//print error , return (NULL);
}

char	*make_pwd_path(char *command, char *pwd)
{
	char	*str;

    if (!pwd)
        return (strjoin_with_free("", command, NO_FREE));//不要?
    str = NULL;
    str = strjoin_with_free(pwd, "/", NO_FREE);
    str = strjoin_with_free(str, command, FREE_S1);
    return (str);
}

char    *getenv_str(t_env *env, char *str)
{
    t_env   *tmp;

    if (!env)
        return (NULL);
    tmp = env;
    while (tmp->key)
    {
        if (!ft_memcmp(tmp->key, str, ft_strlen(str) + 1))
            return (tmp->value);
        tmp = tmp->next;
    }
    return (NULL);
}
