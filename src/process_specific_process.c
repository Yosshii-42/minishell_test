/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_specific_process.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-26 06:38:46 by yotsurud          #+#    #+#             */
/*   Updated: 2024-11-26 06:38:46 by yotsurud         ###   ########i         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	execve_fail_process()
// {

// }

int    no_pipe_process(t_cmd *cmd, int *stdio)
{
    int status;

    cmd->count = 1;
    status = parent_process(cmd, set_token(GET, NULL), NO_PIPE);
    end_status(SET, status);
    syntax_end(cmd, stdio);
    return (status);
}