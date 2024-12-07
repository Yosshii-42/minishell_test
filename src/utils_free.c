/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 06:49:30 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/07 01:30:37 by tsururukako      ###   ########.fr       */
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
			env = NULL;
			break ;
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
		{
			free(token->word);
			token->word = NULL;
		}
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
	{
		free(split[i]);
		split[i] = NULL;
	}
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
		{
			free(cmd->pathname);
			cmd->pathname = NULL;
		}
		if (cmd->cmd)
			free_split(cmd->cmd);
		if (cmd->path)
			free_split(cmd->path);
		if (cmd->err_msg)
		{
			free(cmd->err_msg);
			cmd->err_msg = NULL;
		}
		// cmd->err_file = NULL;
		cmd->token = NULL;
		free(cmd);
		cmd = NULL;
	}
}

void	free_all(t_cmd *cmd)
{
	free_token(set_token(GET, NULL));
	free_cmd(cmd);
	free(set_line(GET, NULL));
}
