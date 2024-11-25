/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 05:45:20 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/23 21:44:02 by hurabe           ###   ########.fr       */
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

void	builtin_cd(t_cmd *cmd, t_env *env)
{
	char	*target_path;
	char	cwd[PATH_MAX];

	// 引数が多すぎる場合エラー
	if (cmd->cmd[2])
	{
		ft_printf(2, "bash: cd: too many arguments\n");
		end_status(SET, EXIT_FAILURE);
	}
	// 移動パス決定
	// 引数がない場合、HOMEへ移動する
	if (!cmd->cmd[1])
		target_path = getenv_str("HOME");
	else if (ft_strncmp(cmd->cmd[1], "-", 2) == 0)
		target_path = getenv_str("OLDPWD");
	else
		target_path = cmd->cmd[1];

	if (!target_path)
	{
		if (cmd->cmd[1] && ft_strncmp(cmd->cmd[1], "-", 2) == 0)
			ft_printf(2, "bash: cd: OLDPWD not set\n");
		else
			ft_printf(2, "bash: cd: HOME not set\n");
		end_status(SET, EXIT_FAILURE);
	}

	// ディレクトリ移動処理
	if (chdir(target_path) == -1)
	{
		ft_printf(2, "bash: cd: %s: %s\n", target_path, strerror(errno));
		end_status(SET, EXIT_FAILURE);
	}
	// PWDとOLDPWDを更新する
	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_printf(2, "bash: cd: error retrieving current directory\n");
		end_status(SET, EXIT_FAILURE);
	}
    update_env_var(env, "PWD", cwd);
	update_env_var(env, "OLDPWD", getenv_str("PWD"));
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
	// 取得に失敗した場合
	ft_printf(2, "bash: pwd: error retrieving current directory: %s\n", strerror(errno));
	end_status(SET, EXIT_FAILURE);
}

void	builtin_export(void)
{
	end_status(SET, EXIT_SUCCESS);
}

// bool	do_builtin(t_cmd *cmd, t_env *env)
bool	do_builtin(t_cmd *cmd)
{
	int		type;
	t_env	*env;

	type = check_builtin(cmd->cmd[0]);
	env = set_get_env(GET, NULL);
	if (type == ECHO)
		return (builtin_echo(cmd), true);
	else if (type == CD)
		return (builtin_cd(cmd, set_get_env(GET, NULL)), true);
	else if (type == PWD)
		return (builtin_pwd(), true);
	else if (type == EXPORT)
		return (builtin_export(), true);//cmd, env));
	else if (type == UNSET)
		// return (builtin_unset(cmd, &env), true);
		return (builtin_unset(cmd, &env), true);
	else if (type == ENV)
		// return (builtin_env(env), true);
		return (builtin_env(), true);
	else if (type == EXIT)
		return (builtin_exit(cmd), false);
	return (true);
}
