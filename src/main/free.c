/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 06:49:30 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/11 18:01:23 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		if (env->key)
			free_string(env->key);
		if (env->value)
			free_string(env->value);
		if (env->next)
		{
			tmp = env;
			env = env->next;
			free(tmp);
		}
		else
		{
			free(env);
			env = NULL;
		}
	}
}

void	free_token(t_token *token)
{
	if (!token)
		return ;
	while (token)
	{
		if (token->kind == LIMITTER)
			unlink(FILE_NAME);
		if (token->word)
			free_string(token->word);
		if (token->next)
		{
			token = token->next;
			free(token->pre);
			token->pre = NULL;
		}
		else
		{
			free(token);
			token = NULL;
			break ;
		}
	}
}

void	free_split(char **split)
{
	int	i;

	i = -1;
	if (!split[0])
	{
		free(split);
		split = NULL;
		return ;
	}
	while (split[++i])
		free_string(split[i]);
	free(split);
	split = NULL;
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd)
	{
		if (cmd->pathname)
			free_string(cmd->pathname);
		if (cmd->cmd)
			free_split(cmd->cmd);
		if (cmd->path)
			free_split(cmd->path);
		if (cmd->err_msg)
			free_string(cmd->err_msg);
		cmd->token = NULL;
		free(cmd);
		cmd = NULL;
	}
}

void	free_all(t_cmd *cmd)
{
	free_token(set_token(GET, NULL));
	free_cmd(cmd);
}
