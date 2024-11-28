/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_specific_process.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 06:38:46 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/28 18:09:07 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execve_fail_process(t_cmd *cmd)
{
	if (cmd->cmd[0][0] == '.')
	{
		ft_printf(2, "bash: %s: %s\n", cmd->cmd[0], strerror(ENOENT));
		free_cmd(cmd);
		exit (127);
	}
	else
	{
		ft_printf(2, "bash: %s: %s\n", cmd->pathname, strerror(EISDIR));
		free_cmd(cmd);
		exit (126);
	}
}

int	no_pipe_process(t_cmd *cmd, int *stdio)
{
	int	status;

    cmd->count = 1;
    status = parent_process(cmd, NO_PIPE);
    end_status(SET, status);
    syntax_end(cmd, stdio);
    return (status);
}
