#include "../minishell.h"

void	init_cmd(t_cmd *cmd)
{
	// cmd->readfd = -1;
	// cmd->writefd = -1;
	cmd->pp[0] = -1;
	cmd->pp[1] = -1;
	cmd->pathname = NULL;
	cmd->cmd = NULL;
	cmd->err_msg = NULL;
}

void    safe_pipe(t_cmd *cmd)
{
    if (pipe(cmd->pp) == -1)
        print_error_and_exit(strerror(errno));
}

char	*make_pwd_path(char *command)
{
	char	*str;

    if (!getenv("PWD"))
        return (strjoin_with_free("", command, NO_FREE));
    str = NULL;
    str = strjoin_with_free(getenv("PWD"), "/", NO_FREE);
    str = strjoin_with_free(str, command, FREE_S1);
    return (str);
}
