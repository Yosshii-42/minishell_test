/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 05:45:20 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/20 16:04:08 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// 仮のもの
bool	print_dolquestion(char *line, int *status)
{
	if (!ft_strncmp(line, "$\?", 2))
	{
		ft_putnbr_fd(*status, 2);
		ft_printf(2, ": command not found\n");
		return (*status = 127, false);
	}
	else
		return (true);
} 

int	builtin_echo(t_cmd *cmd)
{
	int	i;

	if (!cmd->cmd[1])
		return (EXIT_SUCCESS);
	i = 0;
	while (cmd->cmd[++i])
	{
		ft_printf(1, "%s", cmd->cmd[i]);
		if (cmd->cmd[i + 1])
			ft_printf(1, " ");
		else
			ft_printf(1, "\n");
	}
	return (EXIT_SUCCESS);
}

int	builtin_cd(void)
{
	return (EXIT_SUCCESS);
}

int	builtin_pwd(void)
{
	ft_printf(1, "%s\n", getenv("PWD"));
	return (EXIT_SUCCESS);
}

int	builtin_export(void)//t_env *env)
{
	return (EXIT_SUCCESS);
}

int	do_builtin(t_cmd *cmd, t_env *env)
{
	int	type;

	type = check_builtin(cmd->cmd[0]);
	if (type == ECHO)
		return (builtin_echo(cmd));
	else if (type == CD)
		return (builtin_cd());
	else if (type == PWD)
		return (builtin_pwd());
	else if (type == EXPORT)
		return (builtin_export());//cmd, env));
	else if (type == UNSET)
		return (builtin_unset());//cmd, env));
	else if (type == ENV)
		return (builtin_env(env));
	else if (type == EXIT)
		return (builtin_exit(cmd));
	return (EXIT_FAILURE);
}
