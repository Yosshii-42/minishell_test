/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-11 09:20:17 by yotsurud          #+#    #+#             */
/*   Updated: 2024-12-11 09:20:17 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env_node(t_env *env)
{
	if (env->key)
	{
		free(env->key);
		env->key = NULL;
	}
	if (env->value)
	{
		free(env->value);
		env->value = NULL;
	}
	free(env);
	env = NULL;
}

static void	search_and_delete(char *arg, t_env *env, t_env *delete)
{
	while (env->next)
	{
		if (ft_memcmp(arg, delete->key, ft_strlen(delete->key) + 1) == 0)
		{
			env->next = delete->next;
			free_env_node(delete);
			break ;
		}
		env = env->next;
		delete = env->next;
	}
}

void	builtin_unset(t_cmd *cmd)
{	
	int		i;
	t_env	*ptr;
	t_env	*head;

	i = 0;
	while (cmd->cmd[++i])
	{
		head = set_env(GET, NULL);
		ptr = head->next;
		if (ft_memcmp(cmd->cmd[i], head->key, ft_strlen(head->key) + 1) == 0)
		{
			free_env_node(head);
			set_env(SET, ptr);
			return ;
		}
		else
			search_and_delete(cmd->cmd[i], head, ptr);
	}
	end_status(SET, EXIT_SUCCESS);
}

void	builtin_env(void)
{
	t_env	*env;

	env = set_env(GET, NULL);
	while (env)
	{
		if (env->value)
		{
			ft_printf(1, "%s=", env->key);
			if (*(env->value))
				ft_printf(1, "%s", env->value);
			ft_printf(1, "\n");
		}
		env = env->next;
	}
	end_status(SET, EXIT_SUCCESS);
}
