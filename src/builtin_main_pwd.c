/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main_pwd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 01:23:40 by tsururukako       #+#    #+#             */
/*   Updated: 2024/12/11 18:00:13 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

bool	result_of_exit(t_cmd *cmd)
{
	if (cmd->pipe_flag == 1)
		return (ft_printf(2, "exit\n"), false);
	else if (cmd->pipe_flag == -1 && cmd->pp[0] < 0)
		return (ft_printf(2, "exit\n"), true);
	else
		return (true);
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
		return (builtin_unset(cmd), true);
	else if (type == ENV)
		return (builtin_env(), true);
	else if (type == EXIT && builtin_exit(cmd))
		return (result_of_exit(cmd));
	return (true);
}
