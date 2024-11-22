/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_ends.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-20 05:20:14 by yotsurud          #+#    #+#             */
/*   Updated: 2024-11-20 05:20:14 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	end_process(t_token *token, int stdio[2])
{
	free_token(token);
	dup2(stdio[0], STDIN_FILENO);
	dup2(stdio[1], STDOUT_FILENO);
	close(stdio[0]);
	close(stdio[1]);
}

void	syntax_end(t_cmd *cmd, t_token *token, int stdio[2])
{
	if (cmd)
		free_cmd(cmd);
	end_process(token, stdio);
	// free_token(token);
	// dup2(stdio[0], STDIN_FILENO);
	// dup2(stdio[1], STDOUT_FILENO);
	// close(stdio[0]);
	// close(stdio[1]);
}

void	child_exit_process(t_cmd *cmd, int stdio[2])
{
	if (cmd->status != SYNTAX && cmd->err_msg)
		ft_printf(2, "%s", cmd->err_msg);
	close_fds(cmd);
	close(stdio[0]);
	close(stdio[1]);
	if (!ft_strnstr(cmd->err_msg, "Permission", 10))
		exit(126);
	else
		exit(127);
}

int	builtin_end_process(t_cmd *cmd)
{
	if (cmd->status != SYNTAX && cmd->err_msg)
		ft_printf(2, "%s", cmd->err_msg);
	close_fds(cmd);
	if (!ft_strnstr(cmd->err_msg, "Permission", 10))
		return  (126);
	else
		return  (127);
}
