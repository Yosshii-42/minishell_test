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

	cmd->count = 1;
	status = parent_process(cmd, NO_PIPE);
	end_status(SET, status);
	free_cmd(cmd);
	free_token(set_token(GET, NULL));
	end_process(stdio);
	return (status);
}
