/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:49:03 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/25 23:43:09 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_echo(t_cmd *cmd)
{
	int	i;

	if (!cmd->cmd[1])
		end_status(SET, EXIT_SUCCESS);
	i = 0;
	while (cmd->cmd[++i])
	{
		if (!ft_memcmp(cmd->cmd[1], "-n", 3))
			i++;
		if (cmd->cmd[i])
			ft_printf(1, "%s", cmd->cmd[i]);
		if (cmd->cmd[i + 1])
			ft_printf(1, " ");
		else if (ft_memcmp(cmd->cmd[1], "-n", 3))
			ft_printf(1, "\n");
	}
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

// void	exchange_value(t_env *env, char *str)
// {
// 	free(env->value);
// 	env->value = NULL;
// 	env->value = ft_strdup(ft_strchr(str, '=') + 1)
// }

void	builtin_export(void)//t_cmd *cmd)
{
	// t_env	*env;

	// env = set_env(GET, NULL);
	// if (!cmd->cmd[1])
	// {
	// 	while (env)
	// 	{
	// 		ft_printf(1, "declare -x %s", env->key);
	// 		if (env->value)
	// 			ft_printf(1, "=\"%s\"", env->value);
	// 		ft_printf(1, "\n");
	// 		env = env->next;
	// 	}
	// }
	// else if (ft_strchr(cmd->cmd[1], '='))
	// {
	// 	env = set_env(GET, NULL);
	// 	while (env)
	// 	{
	// 		if (ft_strncmd(cmd->cmd[1], env->key, strchr_len(cmd->cmd[1], '=')) == 61)
	// 		{
	// 			exchange_value(env, cmd->cmd[1]);
	// 			break;
	// 		}
	// 		env = env->next;
	// 	}
	// }
	end_status(SET, EXIT_SUCCESS);
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
		return (builtin_export(), true);//cmd), true);
	else if (type == UNSET)
		return (builtin_unset(cmd, &env), true);
	else if (type == ENV)
		return (builtin_env(), true);
	else if (type == EXIT)
		return (builtin_exit(cmd), false);
	return (true);
}
