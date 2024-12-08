/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 07:19:29 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/08 16:58:46 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	lstadd_front(t_env **start, t_env *new)
{
	if (new)
	{
		new->next = *start;
		*start = new;
	}
}

static int	lstnew_export(t_env **start, char *env)
{
	t_env	*new;
	int		len;

	new = (t_env *)safe_malloc(1, sizeof(t_env));
	len = 0;
	if (ft_strchr(env, '='))
		len = strchr_len(env, '=');
	else
		len = ft_strlen(env);
	new->key = (char *)safe_malloc(len + 1, sizeof(char));
	ft_strlcpy(new->key, env, len + 1);
	if (ft_strchr(env, '='))
		new->value = ft_strdup((ft_strchr(env, '=') + 1));
	new->next = NULL;
	lstadd_front(start, new);
	return (TRUE);
}

static void	print_export(void)
{
	t_env	*env;

	env = set_env(GET, NULL);
	while (env)
	{
		ft_printf(1, "declare -x %s", env->key);
		if (env->value)
			ft_printf(1, "=\"%s\"", env->value);
		ft_printf(1, "\n");
		env = env->next;
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

void	builtin_export(t_env **start, t_cmd *cmd)
{
	t_env	*env;
	int		i;

	if (!cmd->cmd[1])
		print_export();
	i = 0;
	while (cmd->cmd[++i])
	{
		env = set_env(GET, NULL);
		while (env)
		{
			if (find_key(cmd->cmd[i], env->key) == true
				&& ft_strchr(cmd->cmd[1], '='))
			{
				free(env->value);
				env->value = ft_strdup(ft_strchr(cmd->cmd[i], '=') + 1);
				end_status(SET, EXIT_SUCCESS);
				return ;
			}
			env = env->next;
		}
		lstnew_export(start, cmd->cmd[i]);
		set_env(SET, *start);
	}
	end_status(SET, EXIT_SUCCESS);
}
