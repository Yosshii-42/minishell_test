/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_specific_process.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-11 09:21:35 by yotsurud          #+#    #+#             */
/*   Updated: 2024-12-11 09:21:35 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	no_fork_process(t_cmd *cmd, int *stdio)
{
	int	status;

	status = 0;
	cmd->count = 1;
	if (cmd->heredoc_sigint == false)
	{
		status = parent_process(cmd, NO_PIPE);
		end_status(SET, status);
	}
	else
		status = 130;
	free_cmd(cmd);
	free_token(set_token(GET, NULL));
	end_process(stdio);
	return (status);
}
