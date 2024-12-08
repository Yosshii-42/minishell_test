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

void	delete_node(t_env *env, t_env **head)
{
	t_env	*tmp;

	tmp = env;
	if (env->pre && env->next)
	{
		env->pre->next = env->next;
		env->next->pre = env->pre;
	}
	else if (env->pre == NULL && env->next)
	{
		env->next->pre = NULL;
		*head = env->next;
		set_env(SET, *head);
	}
	else if (env->pre && env->next == NULL)
		env->pre->next = NULL;
	free(tmp->key);
	free(tmp->value);
	free(tmp);
}

void	builtin_unset(t_cmd *cmd)//, t_env **env)
{
	int		i;
	t_env	**ptr;

	if (!cmd->cmd[1])
	{
		end_status(SET, EXIT_SUCCESS);
		return ;
	}
	i = 0;
	ptr = NULL;
	while (cmd->cmd[++i])
	{
		*ptr = set_env(GET, NULL);
		while ((*ptr))
		{
			if (!ft_memcmp((*ptr)->key, cmd->cmd[i], ft_strlen((*ptr)->key + 1)))
			{
				delete_node(*ptr, ptr);
				break ;
			}
			*ptr = (*ptr)->next;
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
