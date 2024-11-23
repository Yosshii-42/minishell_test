/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-23 06:49:30 by yotsurud          #+#    #+#             */
/*   Updated: 2024-11-23 06:49:30 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env(t_env *env)
{
	while (env)
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
		if (env->next)
		{
			env = env->next;
			free(env->pre);
		}
		else
		{
			free(env);
			break ;
		}
	}
}

void	free_token(t_token *token)
{
	while (token)
	{
		if (token->kind == LIMITTER)
			unlink(FILE_NAME);
		if (token->word)
			free(token->word);
		if (token->next)
		{
			token = token->next;
			free(token->pre);
		}
		else
		{
			free(token);
			break ;
		}
	}
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split || !split[0])
		return ;
	while (split[i])
		free(split[i++]);
	free(split);
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd->pathname)
			free(cmd->pathname);
		if (cmd->cmd)
			free_split(cmd->cmd);
		if (cmd->err_msg)
			free(cmd->err_msg);
		free(cmd);
	}
}
