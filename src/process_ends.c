/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_ends.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 05:20:14 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/26 00:48:26 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	end_process(int stdio[2])
{
	free_token(set_token(GET, NULL));
	dup2(stdio[0], STDIN_FILENO);
	dup2(stdio[1], STDOUT_FILENO);
	close(stdio[0]);
	close(stdio[1]);
}

void	syntax_end(t_cmd *cmd, int stdio[2])
{
	if (cmd)
		free_cmd(cmd);
	end_process(stdio);
}

void	child_exit_process(t_cmd *cmd, int stdio[2])
{
	char	*find_permission;

	if (!cmd->cmd)
		exit(EXIT_SUCCESS);
	// if (cmd->status != SYNTAX && cmd->err_msg)
	ft_printf(2, "%s", cmd->err_msg);
	find_permission = ft_strnstr(cmd->err_msg, "Permission", 10);
	close_fds(cmd);
	close(stdio[0]);
	close(stdio[1]);
	free_cmd(cmd);
	// free_token(token);
	free_token(set_token(GET, NULL));
	if (find_permission != NULL)
		exit(126);
	else
		exit(127);
}

int	builtin_end_process(t_cmd *cmd, t_token *token)
{
	char	*find_permission;
	
	// if (cmd->status != SYNTAX && cmd->err_msg)
	ft_printf(2, "%s", cmd->err_msg);
	find_permission = ft_strnstr(cmd->err_msg, "fPermission", 10);
	close_fds(cmd);
	free_cmd(cmd);
	free_token(token);
	if (find_permission != NULL)
		return  (126);
	else
		return  (127);
}
