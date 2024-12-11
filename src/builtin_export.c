/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-11 09:19:56 by yotsurud          #+#    #+#             */
/*   Updated: 2024-12-11 09:19:56 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	print_export(void)
{
	t_env	*env;

	env = set_env(GET, NULL);
	while (env)
	{
		if (ft_memcmp(env->key, "_", 2) == 0)
			env = env->next;
		if (env)
		{
			ft_printf(1, "declare -x %s", env->key);
			if (env->value)
				ft_printf(1, "=\"%s\"", env->value);
			ft_printf(1, "\n");
			env = env->next;
		}
	}
}

bool	find_key(char *command, char *key)
{
	if (ft_strchr(command, '='))
	{
		if (ft_memcmp(command, key, strchr_len(command, '=') + 1) == '=')
			return (true);
	}
	else
	{
		if (ft_memcmp(command, key, ft_strlen(command) + 1) == 0)
			return (true);
	}
	return (false);
}

static bool	check_cmd(char *command)
{
	if (*command >= '0' && *command <= '9')
		return (false);
	while (*command && *command != '=')
	{
		if (ft_strchr(INVALID_CHAR, *command))
			return (false);
		command++;
	}
	return (true);
}

void	export_process(t_env **start, char *str)
{
	t_env	*env;

	env = set_env(GET, NULL);
	while (env)
	{
		if (find_key(str, env->key) == true
			&& ft_strchr(str, '='))
		{
			free(env->value);
			env->value = ft_strdup(ft_strchr(str, '=') + 1);
			end_status(SET, EXIT_SUCCESS);
			return ;
		}
		else if (find_key(str, env->key) == true)
			return ;
		env = env->next;
	}
	lstnew_export(start, str);
	set_env(SET, *start);
}

void	builtin_export(t_env **start, t_cmd *cmd)
{
	int		i;
	int		flag;

	if (!cmd->cmd[1])
		print_export();
	i = 0;
	flag = 0;
	while (cmd->cmd[++i])
	{
		if (check_cmd(cmd->cmd[i]) == false)
		{
			ft_printf(2, "bash: export: %s: ", cmd->cmd[i]);
			ft_printf(2, "not a valid identifier\n");
			flag++;
		}
		else
			export_process(start, cmd->cmd[i]);
	}
	if (flag)
		end_status(SET, EXIT_FAILURE);
	else
		end_status(SET, EXIT_SUCCESS);
}
