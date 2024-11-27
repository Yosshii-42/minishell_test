/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:27:24 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/27 02:38:36 by tsururukako      ###   ########.fr       */
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
		cmd->path = ft_split(getenv_str("PATH"), ':');
	cmd->err_msg = NULL;
	cmd->err_file = NULL;
	cmd->err_no = -1;
	cmd->token = NULL;
	cmd->status = -1;
	cmd->flag = 0;
	return (true);
}

void	make_pipe(t_cmd *cmd)
{
	if (pipe(cmd->pp) == -1)
		exit((ft_printf(2, "pipe: %s\n", strerror(errno)), EXIT_FAILURE));
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

char	*getenv_str(char *str)
{
	t_env	*env;
	t_env	*tmp;

	env = set_env(GET, NULL);
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

t_cmd	*command_return(t_cmd *cmd, t_token *token)
{
	if (cmd->pathname && access(cmd->pathname, X_OK) != 0)
		set_err_message(cmd, cmd->cmd[0], strerror(errno));
	cmd->token = token;
	return (cmd);
}
