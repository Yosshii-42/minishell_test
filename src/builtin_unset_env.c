/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:55:33 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/28 18:18:15 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_unset(t_cmd *cmd, t_env **env)
{
	if (!cmd->cmd[1])
		end_status(SET, EXIT_SUCCESS);
	while ((*env))
	{
		if (!ft_memcmp((*env)->key, cmd->cmd[1], ft_strlen(cmd->cmd[1]) + 1))
		{
			if ((*env)->pre)
				(*env)->pre->next = (*env)->next;
			else
			{
				if ((*env)->next)
				{
					(*env)->next->pre = NULL;
					env = &(*env)->next;
				}
			}
			free((*env)->key);
			free((*env)->value);
			free(*env);
			end_status(SET, EXIT_SUCCESS);
		}
		*env = (*env)->next;
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
