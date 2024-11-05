/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-05 06:27:09 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/05 15:57:30 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	set_err_message(t_cmd *cmd, char *str)
{
	if (cmd->err_msg)
		return ;
	cmd->err_msg = NULL;
	if (!str || !*str)
		cmd->err_msg = strjoin_with_free("", "Command ' ' not found\n", NO_FREE);
	else
	{
		cmd->err_msg = strjoin_with_free("bash: ", str, NO_FREE);
		cmd->err_msg = strjoin_with_free(cmd->err_msg,
				": command not found\n", FREE_S1);
	}
}

static char	*make_cmd_and_check_access(char *command, char **path, char *pwd)
{
	char	*str;
	int		i;

	str = NULL;
	if (!path)
		return (make_pwd_path(command, pwd));
	i = -1;
	while (path[++i])
	{
		str = strjoin_with_free(path[i], "/", NO_FREE);
		str = strjoin_with_free(str, command, FREE_S1);
		if (!access(str, X_OK))
			return (str);
		free(str);
		str = NULL;
	}
	return (strjoin_with_free("x", command, NO_FREE));
}

static t_cmd	*make_command_array(t_token *token, t_cmd *cmd)
{
	t_token	*ptr;
	int		count;
	int		i;

	ptr = token;
	count = array_count(token);
	cmd->cmd = (char **)malloc(sizeof(char *) * (count + 1));
	if (!(cmd->cmd))
		return (NULL);
	i = -1;
	while (++i < count)
	{
		cmd->cmd[i] = ft_strdup(token->word);
		if (!cmd->cmd[i])
			return (free_split(cmd->cmd), NULL);
		token = token->next;
	}
	cmd->cmd[i] = NULL;
	cmd->token = ptr;
	return (cmd);
}

static void	make_path_and_cmd(t_token *token, t_cmd *cmd, char **path,
		char *pwd)
{
	cmd = make_command_array(token, cmd);
	if (!cmd)
		return ;//exit (1); //TODO error_exit
	if ((cmd->cmd[0]))
	{
		if (cmd->cmd[0][0] == '/')
			cmd->pathname = strjoin_with_free("", cmd->cmd[0], NO_FREE);
		else if (cmd->cmd[0][0] == '.')
			cmd->pathname = make_pwd_path(cmd->cmd[0], pwd);
		else
			cmd->pathname = make_cmd_and_check_access(cmd->cmd[0], path, pwd);
		if (cmd->pathname)
			return ;
	}
	cmd->pathname = strjoin_with_free("", cmd->cmd[0], NO_FREE);
}

t_cmd	*make_cmd(t_token *token, t_cmd *cmd, char **path, char *pwd)
{
	cmd = (t_cmd *)malloc(sizeof (t_cmd));
	if (!cmd)
		return (NULL);
	init_cmd(cmd);
	while (token)
	{
		if (token->kind == SYNTAX)
		{
			cmd->status = SYNTAX;
			break ;
		}
		if (token->kind == PIPE)
		{
			if (!make_pipe(cmd))
				return (free_cmd(cmd), NULL);
			token = token->next;
			break ;
		}
		else if (token->kind == RDFILE || token->kind == LIMITTER)
			open_read_file(cmd, token);
		else if (token->kind == WRFILE || token->kind == WRF_APP)
			open_write_file(cmd, token);
		else if (token->kind == COMMAND)
		{
			make_path_and_cmd(token, cmd, path, pwd);
			token = cmd->token;
		}
		if (token->status != END)
			token = token->next;
		else
			break ;
	}
	if (cmd-> pathname && access(cmd->pathname, X_OK) != 0)
		set_err_message(cmd, cmd->cmd[0]);
	cmd->token = token;
	return (cmd);
}
