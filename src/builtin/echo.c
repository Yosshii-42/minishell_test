/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-11 09:19:15 by yotsurud          #+#    #+#             */
/*   Updated: 2024-12-11 09:19:15 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	echo_without_option(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->cmd[++i])
	{
		ft_printf(1, "%s", cmd->cmd[i]);
		if (cmd->cmd[i + 1])
			ft_printf(1, " ");
	}
	ft_printf(1, "\n");
}

static bool	is_all_n(char *option)
{
	int	i;

	i = 1;
	while (option[i])
	{
		if (option[i] == 'n')
			i++;
		else
			return (false);
	}
	return (true);
}

static void	echo_n_option(t_cmd *cmd)
{
	int	i;
	int	all_n_flag;

	i = 1;
	all_n_flag = 0;
	while (cmd->cmd[i])
	{
		if (!ft_strncmp(cmd->cmd[i], "-n", 2) && is_all_n(cmd->cmd[i]) == true)
		{
			all_n_flag++;
			i++;
		}
		else
			break ;
	}
	while (cmd->cmd[i])
	{
		ft_printf(1, "%s", cmd->cmd[i]);
		if (cmd->cmd[i + 1])
			ft_printf(1, " ");
		i++;
	}
	if (all_n_flag == 0)
		ft_printf(1, "\n");
}

void	builtin_echo(t_cmd *cmd)
{
	if (!cmd->cmd[1])
	{
		end_status(SET, EXIT_SUCCESS);
		ft_printf(1, "\n");
		return ;
	}
	if (!ft_strncmp(cmd->cmd[1], "-n", 2))
		echo_n_option(cmd);
	else
		echo_without_option(cmd);
	end_status(SET, EXIT_SUCCESS);
}
