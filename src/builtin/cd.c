/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:15:09 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/08 18:32:32 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	bool	builtin_cd_check(char **cmd, char *oldpwd)
{
	int	count;

	count = count_arguments(cmd);
	if (count > 2)
	{
		ft_printf(2, "bash: cd: too many arguments\n");
		end_status(SET, EXIT_FAILURE);
		return (false);
	}
	if (!getcwd(oldpwd, PATH_MAX))
	{
		ft_printf(2, "bash: cd: error retrieving current directory\n");
		end_status(SET, EXIT_FAILURE);
		return (false);
	}
	return (true);
}

static	char	*resolve_cd_path(t_cmd *cmd)
{
	char	*target_path;

	if (!cmd->cmd[1])
		return (getenv_str("HOME"));
	if (cmd->cmd[1][0] == '\0')
	{
		end_status(SET, EXIT_FAILURE);
		return (NULL);
	}
	target_path = skip_spaces(cmd->cmd[1]);
	if (*target_path == '\0')
	{
		ft_printf(2, "bash: cd: %s: No such file or directory\n", cmd->cmd[1]);
		end_status(SET, EXIT_FAILURE);
		return (NULL);
	}
	if (ft_memcmp(cmd->cmd[1], "-", 2) == 0)
	{
		ft_printf(2, "bash: cd: -: not supported\n");
		end_status(SET, EXIT_FAILURE);
		return (NULL);
	}
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
		end_status(SET, EXIT_FAILURE);
		ft_strlcpy(cwd, target_path, PATH_MAX);
		return (false);
	}
	return (true);
}

static void	update_env_var(t_env *env, const char *key, const char *value)
{
	t_env	*current;
	t_env	*newnode;

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
	newnode = (t_env *)safe_malloc(1, sizeof(t_env));
	newnode->key = ft_strdup(key);
	newnode->value = ft_strdup(value);
	newnode->next = NULL;
	lstadd_back(&env, newnode);
}

void	builtin_cd(t_cmd *cmd, t_env *env)
{
	char	*target_path;
	char	cwd[PATH_MAX];
	char	oldpwd[PATH_MAX];
	bool	status;

	if (!builtin_cd_check(cmd->cmd, oldpwd))
		return ;
	target_path = resolve_cd_path(cmd);
	if (!target_path)
		return ;
	status = change_directory(target_path, cwd);
	if (status)
	{
		update_env_var(env, "OLDPWD", oldpwd);
		update_env_var(env, "PWD", cwd);
		end_status(SET, EXIT_SUCCESS);
	}
	else
		end_status(SET, EXIT_FAILURE);
}
