/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-05 06:27:24 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/05 15:45:13 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	array_count(t_token *token)
{
	int	count;

	count = 0;
	while (token->kind == COMMAND || token->kind == OPTION)
	{
		count++;
		if (token->status != END && token->next->kind == OPTION)
			token = token->next;
		else
			break ;
	}
	return (count);
}

void	init_cmd(t_cmd *cmd)
{
	cmd->readfd = -1;
	cmd->writefd = -1;
	cmd->count = 0;
	cmd->pp[0] = -1;
	cmd->pp[1] = -1;
	cmd->pathname = NULL;
	cmd->cmd = NULL;
	cmd->err_msg = NULL;
	cmd->token = NULL;
	cmd->status = -1;
}

int	make_pipe(t_cmd *cmd)
{
	if (pipe(cmd->pp) == -1)
		return (ft_printf(2, "%s\n", strerror(errno)), FALSE);
	return (TRUE);
}

char	*make_pwd_path(char *command, char *pwd)
{
	char	*str;

	if (!pwd)
		return (strjoin_with_free("", command, NO_FREE));//不要?
	str = NULL;
	str = strjoin_with_free(pwd, "/", NO_FREE);
	str = strjoin_with_free(str, command, FREE_S1);
	return (str);
}

char	*getenv_str(t_env *env, char *str)
{
	t_env	*tmp;

	if (!env)
		return (NULL);
	tmp = env;
	while (tmp->key)
	{
		if (!ft_memcmp(tmp->key, str, ft_strlen(str) + 1))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}
