/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main_echo_pwd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:49:03 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/05 23:27:17 by tsururukako      ###   ########.fr       */
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

static void	echo_n_option(t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->cmd[i])
	{
		if (!ft_strncmp(cmd->cmd[i], "-n", 2))
			i++;
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

void	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
	{
		ft_printf(1, "%s\n", cwd);
		end_status(SET, EXIT_SUCCESS);
	}
	else
	{
		ft_printf(2, "bash: pwd: error retrieving current directory: ");
		ft_printf(2, "%s\n", strerror(errno));
		end_status(SET, EXIT_FAILURE);
	}
}

bool	do_builtin(t_cmd *cmd)
{
	int		type;
	t_env	*env;

	type = check_builtin(cmd->cmd[0]);
	env = set_env(GET, NULL);
	if (type == ECHO)
		return (builtin_echo(cmd), true);
	else if (type == CD)
		return (builtin_cd(cmd, set_env(GET, NULL)), true);
	else if (type == PWD)
		return (builtin_pwd(), true);
	else if (type == EXPORT)
		return (builtin_export(&env, cmd), true);
	else if (type == UNSET)
		return (builtin_unset(cmd, &env), true);
	else if (type == ENV)
		return (builtin_env(), true);
	else if (type == EXIT)
		return (builtin_exit(cmd), false);
	return (true);
}
