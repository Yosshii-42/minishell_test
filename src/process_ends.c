/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_ends.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 05:20:14 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/28 18:08:10 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	end_process(int stdio[2])
{
	// free_token(set_token(GET, NULL));
	if (dup2(stdio[0], STDIN_FILENO) == -1)
		exit((ft_printf(2, "dup2: %s\n", strerror(errno)), EXIT_FAILURE));
	if (dup2(stdio[1], STDOUT_FILENO) == -1)
		exit((ft_printf(2, "dup2: %s\n", strerror(errno)), EXIT_FAILURE));
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

	close_fds(cmd);
	close(stdio[0]);
	close(stdio[1]);
	if (cmd->err_msg)
		ft_printf(2, "%s", cmd->err_msg);
	if (!cmd->pathname)
		exit((free_cmd(cmd), free_token(set_token(GET, NULL)), EXIT_SUCCESS));
	find_permission = ft_strnstr(cmd->err_msg, "Permission", 10);
	free_cmd(cmd);
	if (find_permission != NULL)
		exit(126);
	else
		exit(127);
}

int	builtin_end_process(t_cmd *cmd)
{
	char	*find_permission;

	close_fds(cmd);
	ft_printf(2, "%s", cmd->err_msg);
	find_permission = ft_strnstr(cmd->err_msg, "fPermission", 10);
	if (cmd)
		free_cmd(cmd);
	if (set_token(GET, NULL))
		free_token(set_token(GET, NULL));
	if (find_permission != NULL)
		return (126);
	else
		return (127);
}
