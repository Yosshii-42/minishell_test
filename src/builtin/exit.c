/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 09:19:33 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/11 23:48:29 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	normal_exit(t_cmd *cmd, long result)
{
	if (cmd->pipe_flag == 1)
		ft_printf(2, "exit\n");
	end_status(SET, result % 256);
}

void	numeric_error(t_cmd *cmd)
{
	if (cmd->pipe_flag == 1)
		ft_printf(2, "exit\n");
	ft_printf(2, "bash: exit: %s", cmd->cmd[1]);
	ft_printf(2, ": numeric argument required\n");
	end_status(SET, 2);
}

void	too_many_error(t_cmd *cmd)
{
	if (cmd->pipe_flag == 1 || (cmd->pipe_flag == -1 && cmd->pp[0] < 0))
		ft_printf(2, "exit\n");
	ft_printf(2, "bash: exit: too many arguments\n");
	end_status(SET, 1);
	cmd->pipe_flag = -1;
}

bool	builtin_exit(t_cmd *cmd)
{
	int		count;
	long	*result;

	count = -1;
	while (cmd->cmd[++count])
		;
	if (count == 1)
		return (ft_printf(2, "exit\n"), end_status(SET, EXIT_SUCCESS), true);
	result = atol_pointer(cmd->cmd[1]);
	if (count == 2 && !result)
		numeric_error(cmd);
	else if (count == 2)
		normal_exit(cmd, *result);
	else if (count >= 3 && result)
		too_many_error(cmd);
	else
		numeric_error(cmd);
	return (true);
}
