/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_specific_process.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 06:38:46 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/07 00:47:33 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	no_fork_process(t_cmd *cmd, int *stdio)
{
	int	status;

	cmd->count = 1;
	status = parent_process(cmd, NO_PIPE);
	end_status(SET, status);
	free_cmd(cmd);
	free_token(set_token(GET, NULL));
	end_process(stdio);
	return (status);
}
