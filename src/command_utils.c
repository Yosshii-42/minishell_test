/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:27:24 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/09 01:55:10 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	init_cmd(t_cmd *cmd)
{
	cmd->readfd = -1;
	cmd->writefd = -1;
	cmd->count = 0;
	cmd->pp[0] = -1;
	cmd->pp[1] = -1;
	cmd->pathname = NULL;
	cmd->cmd = NULL;
	cmd->path = NULL;
	if (getenv_str("PATH"))
	{
		cmd->path = ft_split(getenv_str("PATH"), ':');
		if (!cmd->path)
			return (ft_printf(2, "malloc: %s\n", strerror(errno)), false);
	}
	cmd->err_msg = NULL;
	cmd->token = NULL;
	cmd->status = -1;
	cmd->flag = 0;
	return (true);
}

int	count_array(t_token *token)
{
	int	count;
	count = 0;
	while (token)
	{
		if (token->kind == COMMAND || token->kind == BUILTIN
			|| token->kind == OPTION)
			count++;
		if (token->kind == PIPE)
			break;
		token = token->next;
	}
	return (count);
}

int	count_token(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		count++;
		if (token->kind == PIPE)
			break;
		token = token->next;
	}
	return (count);
}

int	make_pipe(t_cmd *cmd)
{
	if (pipe(cmd->pp) == -1)
		return (ft_printf(2, "pipe: %s\n", strerror(errno)), FALSE);
	return (TRUE);
}

char	*make_pwd_path(char *command, char *pwd)
{
	char	*str;

	if (!pwd)
		return (strjoin_with_free("", command, NO_FREE));//不要?
	str = NULL;
	str = strjoin_with_free(pwd, "/", NO_FREE);
	if (str)
		str = strjoin_with_free(str, command, FREE_S1);
	if (!str)
		ft_printf(2, "malloc: %s\n", strerror(errno));
	return (str);
}

char	*getenv_str(char *str)
{
	t_env	*env;
	t_env	*tmp;

	env = set_get_env(GET, NULL);
	if (!env)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		if (!ft_memcmp(tmp->key, str, ft_strlen(str) + 1))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}
