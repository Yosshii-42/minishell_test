/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:55:33 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/05 19:42:05 by hurabe           ###   ########.fr       */
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

void	delete_node(t_env *env, t_env *next_env)
{
	free_env_node(env->next);
	env->next = next_env;
}

void	builtin_unset(t_cmd *cmd, t_env **env)
{
	int		i;
	t_env	*ptr;
	// t_env	*tmp;
	t_env	*next_env;

	if (!cmd->cmd[1])
	{
		end_status(SET, EXIT_SUCCESS);
		return ;
	}
	i = 0;
	ptr = *env;
	while (cmd->cmd[++i])
	{
		ptr = *env;
		if (ptr == *env && !ft_memcmp(ptr->key, cmd->cmd[i], ft_strlen(ptr->key) + 1))
		{
			set_env(SET, ptr->next);
			free_env_node(ptr);
			ptr = set_env(GET, NULL);
		}
		else
		{
			while (ptr)
			{
				if (ptr->next)
				{
					next_env = ptr->next;
					if (!ft_memcmp(next_env->key, cmd->cmd[i], ft_strlen(next_env->key + 1)))
					{
						printf("cmd[%d] = %s, env->next->key = %s\n", i, cmd->cmd[i], next_env->key);
						// tmp = ptr->next->next;
						delete_node(ptr, ptr->next->next);
						// ptr = tmp;
						break ;
					}
					ptr = ptr->next;
				}
			}
			// else
			ptr = ptr->next;
		}
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
