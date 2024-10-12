
#include "../minishell.h"

void	close_fds(t_cmd *cmd)
{
	// if (cmd->readfd > 0)
	// 	close(cmd->readfd);
	// if (cmd->writefd > 0)
	// 	close(cmd->writefd);
	close(cmd->pp[0]);
	close(cmd->pp[1]);
}

void	exit_child_process(t_cmd *cmd)
{
	if (cmd->err_msg)
		ft_printf(2, "%s", cmd->err_msg);
	// close_fds(cmd);
	exit(127);
}
