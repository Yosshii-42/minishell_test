/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:55:33 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/09 23:01:22 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_err_and_set_exit_status(char *argument, long *result)
{
	if (result == NULL)
	{
		ft_printf(2, "bash: exit: %s", argument);
		ft_printf(2, ": numeric argument required\n");
		end_status(SET, EXIT_FAILURE);
	}
	else
		end_status(SET, (*result) % 256);
}

void	numeric_error(char *argument)
{
	ft_printf(2, "bash: exit: %s", argument);
	ft_printf(2, ": numeric argument required\n");
	end_status(SET, 2);
}

void	too_many_error(t_cmd *cmd)
{
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
	{
		end_status(SET, EXIT_SUCCESS);
		return (true);
	}
	result = atol_pointer(cmd->cmd[1]);
	if (count == 2 && !result)
		numeric_error(cmd->cmd[1]);
	else if (count == 2)
		end_status(SET, *result);
	else if (count >= 3 && result)
		too_many_error(cmd);
	else
		numeric_error(cmd->cmd[1]);
	return (true);
}
