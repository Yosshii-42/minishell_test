/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:15:09 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/25 23:45:05 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_env_var(t_env *env, char *key, char *value)
{
    t_env *current = env;

    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            free(current->value);
            current->value = ft_strdup(value);
            return;
        }
        current = current->next;
    }

    // 存在しない場合、新しいノードを追加
    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node)
        return;
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = NULL;
    lstadd_back(&env, new_node);
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
