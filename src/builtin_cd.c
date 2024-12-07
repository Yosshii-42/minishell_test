/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:15:09 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/07 15:01:21 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	char	*resolve_cd_path(t_cmd *cmd)
{
	char	*target_path;

	if (!cmd->cmd[1])
		//target_path = getenv_str("HOME");
		return (getenv_str("HOME"));
	if (ft_memcmp(cmd->cmd[1], ".", 2) == 0)
	{
		//printf("test");
		//target_path = getenv_str("PWD");
		ft_printf(2, "DEBUG: cd . -> getenv_str(\"PWD\")\n");
		return (getenv_str("PWD"));
	}
	if (ft_memcmp(cmd->cmd[1], "-", 2) == 0)
		//target_path = getenv_str("OLDPWD");
		return (getenv_str("OLDPWD"));
	target_path = skip_spaces(cmd->cmd[1]);
	if (!*target_path)
		return (getenv_str("HOME"));
	if (cmd->cmd[1][0] == '\0')
		return (end_status(SET, EXIT_SUCCESS), NULL);
	else
		return (cmd->cmd[1]);
	return (cmd->cmd[1]);
}

static	bool	change_directory(const char *target_path, char *cwd)
{
	if (chdir(target_path) == -1)
	{
		ft_printf(2, "bash: cd: %s: %s\n", target_path, strerror(errno));
		end_status(SET, EXIT_FAILURE);
		return (false);
	}
	if (!getcwd(cwd, sizeof(cwd)))
	{
		//ft_printf(2, "bash: cd: error retrieving current directory\n");
		end_status(SET, EXIT_FAILURE);
		ft_strlcpy(cwd, target_path, PATH_MAX);
		return (false);
	}
	return (true);
}

// 環境変数の更新
static void	update_env_var(t_env *env, const char *key, const char *value)
{
	t_env	*current;
	t_env	*newnode;

	current = env;
	while (current)
	{
		// 既存のキーを検索、見つかったら値を更新
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	newnode = (t_env *)safe_malloc(1, sizeof(t_env));
	newnode->key = ft_strdup(key);
	newnode->value = ft_strdup(value);
	newnode->next = NULL;
	// リストの末尾に新しいノードを追加
	lstadd_back(&env, newnode);
}

void	builtin_cd(t_cmd *cmd, t_env *env)
{
	int		count;
	char	*target_path;
	char	cwd[PATH_MAX];
	char	oldpwd[PATH_MAX];

	count = count_arguments(cmd->cmd);
	if (count > 2)
	{
		ft_printf(2, "bash: cd: too many arguments\n");
		end_status(SET, EXIT_FAILURE);
		return ;
	}
	if (!getcwd(oldpwd, PATH_MAX))
	{
		ft_printf(2, "bash: cd: error retrieving current directory\n");
		end_status(SET, EXIT_FAILURE);
		return ;
	}
	target_path = resolve_cd_path(cmd);
	if (!target_path)
		return ;
	if (!change_directory(target_path, cwd))
		return ;
	update_env_var(env, "OLDPWD", oldpwd);
	update_env_var(env, "PWD", cwd); 
	//update_env_var(env, "PWD", cwd);
	//update_env_var(env, "OLDPWD", getenv_str("PWD"));
	end_status(SET, EXIT_SUCCESS);
	ft_printf(2, "DEBUG: cwd=%s, oldpwd=%s\n", cwd, oldpwd);
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

//static char	*resolve_target_path(t_cmd *cmd)
//{
//	if (!cmd->cmd[1])
//		return (getenv_str("HOME"));
//	if (ft_strncmp(cmd->cmd[1], "-", 2) == 0)
//		return (getenv_str("OLDPWD"));
//	return (cmd->cmd[1]);
//}

//static void	handle_path_error(const char *cmd)
//{
//	if (cmd && ft_strncmp(cmd, "-", 2) == 0)
//		ft_printf(2, "bash: cd: OLDPWD not set\n");
//	else
//		ft_printf(2, "bash: cd: HOME not set\n");
//	end_status(SET, EXIT_FAILURE);
//}

//static void	handle_chdir_error(const char *path)
//{
//	ft_printf(2, "bash: cd: %s: %s\n", path, strerror(errno));
//	end_status(SET, EXIT_FAILURE);
//}

//void	builtin_cd(t_cmd *cmd, t_env *env)
//{
//	char	*target_path;
//	char	cwd[PATH_MAX];
//	char	*old_pwd;

//	target_path = resolve_target_path(cmd);
//	if (!target_path)
//		handle_path_error(cmd->cmd[1]);
//	if (chdir(target_path) == -1)
//		handle_chdir_error(target_path);
//	if (!getcwd(cwd, sizeof(cwd)))
//	{
//		ft_printf(2, "bash: cd: error retrieving current directory\n");
//		end_status(SET, EXIT_FAILURE);
//	}
//	old_pwd = getenv_str("PWD");
//	if (cmd->cmd[2])
//	{
//		ft_printf(2, "bash: cd: too many arguments\n");
//		end_status(SET, EXIT_FAILURE);
//	}
//	if (old_pwd)
//		update_env_var(env, "OLDPWD", old_pwd);
//	update_env_var(env, "PWD", cwd);
//	//update_env_var(env, "PWD", cwd);
//	//update_env_var(env, "OLDPWD", getenv_str("PWD"));
//	end_status(SET, EXIT_SUCCESS);
//}


//---------------- 以下初期値 ----------------

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
