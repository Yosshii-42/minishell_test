/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:15:09 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/05 21:10:26 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_env_var(t_env *env, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	new_node = (t_env *)safe_malloc(1, sizeof(t_env));
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	lstadd_back(&env, new_node);
}

static char	*resolve_target_path(t_cmd *cmd)
{
	if (!cmd->cmd[1])
		return (getenv_str("HOME"));
	if (ft_strncmp(cmd->cmd[1], "-", 2) == 0)
		return (getenv_str("OLDPWD"));
	return (cmd->cmd[1]);
}

static void	handle_path_error(const char *cmd)
{
	if (cmd && ft_strncmp(cmd, "-", 2) == 0)
		ft_printf(2, "bash: cd: OLDPWD not set\n");
	else
		ft_printf(2, "bash: cd: HOME not set\n");
	end_status(SET, EXIT_FAILURE);
}

static void	handle_chdir_error(const char *path)
{
	ft_printf(2, "bash: cd: %s: %s\n", path, strerror(errno));
	end_status(SET, EXIT_FAILURE);
}

void	builtin_cd(t_cmd *cmd, t_env *env)
{
	char	*target_path;
	char	cwd[PATH_MAX];
	char	*old_pwd;

	target_path = resolve_target_path(cmd);
	if (!target_path)
		handle_path_error(cmd->cmd[1]);
	if (chdir(target_path) == -1)
		handle_chdir_error(target_path);
	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_printf(2, "bash: cd: error retrieving current directory\n");
		end_status(SET, EXIT_FAILURE);
	}
	old_pwd = getenv_str("PWD");
	if (cmd->cmd[2])
	{
		ft_printf(2, "bash: cd: too many arguments\n");
		end_status(SET, EXIT_FAILURE);
	}
	if (old_pwd)
		update_env_var(env, "OLDPWD", old_pwd);
	update_env_var(env, "PWD", cwd);
	//update_env_var(env, "PWD", cwd);
	//update_env_var(env, "OLDPWD", getenv_str("PWD"));
	end_status(SET, EXIT_SUCCESS);
}


//static void	update_env_var(t_env *env, char *key, char *value)
//{
//	t_env	*current;
//	t_env	*new_node;

//	current = env;
//	while (current)
//	{
//		if (ft_strcmp(current->key, key) == 0)
//		{
//			free(current->value);
//			current->value = ft_strdup(value);
//			return ;
//		}
//		current = current->next;
//	}
//	new_node = (t_env *)safe_malloc(1, sizeof(t_env));
//	new_node->key = ft_strdup(key);
//	new_node->value = ft_strdup(value);
//	new_node->next = NULL;
//	lstadd_back(&env, new_node);
//}

//void	builtin_cd(t_cmd *cmd, t_env *env)
//{
//	char	*target_path;
//	char	cwd[PATH_MAX];

//	if (cmd->cmd[2])
//	{
//		ft_printf(2, "bash: cd: too many arguments\n");
//		end_status(SET, EXIT_FAILURE);
//	}
//	if (!cmd->cmd[1])
//		target_path = getenv_str("HOME");
//	else if (ft_strncmp(cmd->cmd[1], "-", 2) == 0)
//		target_path = getenv_str("OLDPWD");
//	else
//		target_path = cmd->cmd[1];
//	if (!target_path)
//	{
//		if (cmd->cmd[1] && ft_strncmp(cmd->cmd[1], "-", 2) == 0)
//			ft_printf(2, "bash: cd: OLDPWD not set\n");
//		else
//			ft_printf(2, "bash: cd: HOME not set\n");
//		end_status(SET, EXIT_FAILURE);
//	}
//	if (chdir(target_path) == -1)
//	{
//		ft_printf(2, "bash: cd: %s: %s\n", target_path, strerror(errno));
//		end_status(SET, EXIT_FAILURE);
//	}
//	if (!getcwd(cwd, sizeof(cwd)))
//	{
//		ft_printf(2, "bash: cd: error retrieving current directory\n");
//		end_status(SET, EXIT_FAILURE);
//	}
//	update_env_var(env, "PWD", cwd);
//	update_env_var(env, "OLDPWD", getenv_str("PWD"));
//	end_status(SET, EXIT_SUCCESS);
//}
