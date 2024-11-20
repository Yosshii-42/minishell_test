/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:27:09 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/20 18:39:37 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	set_err_message(t_cmd *cmd, char *str)
{
	if (cmd->err_msg)
		return (true);
	cmd->err_msg = NULL;
	if (!str || !*str)
		cmd->err_msg = strjoin_with_free("", "Command ' ' not found\n", NO_FREE);
	else
	{
		cmd->err_msg = strjoin_with_free("bash: ", str, NO_FREE);
		if (cmd->err_msg)
			cmd->err_msg = strjoin_with_free(cmd->err_msg,
					": command not found\n", FREE_S1);
	}
	if (!cmd->err_msg)
		return (false);
	return (true);
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
		if (!str)
			return (NULL);//後で検討
		if (!access(str, X_OK))
			return (str);
		free(str);
		str = NULL;
	}
	return (strjoin_with_free("x", command, NO_FREE));
}

static t_cmd	*make_command_array(t_token *token, t_cmd *cmd)
{
	int		array_count;
	int		token_count;
	int		i;
	int		j;

	array_count = count_array(token);
	token_count = count_token(token);

	cmd->cmd = (char **)malloc(sizeof(char *) * (array_count + 1));
	if (!(cmd->cmd))
		return (NULL);
	i = -1;
	j = -1;
	while (++i < token_count)
	{
		if (token->kind == BUILTIN || token->kind == COMMAND || token->kind == OPTION)
		{
			cmd->cmd[++j] = ft_strdup(token->word);
			if (!cmd->cmd[j])
				return (free_split(cmd->cmd), NULL);
		}
		token = token->next;
	}
	cmd->cmd[array_count] = NULL;
	return (cmd);
}

static bool	make_path_cmd(t_token *token, t_cmd *cmd, char **path)
{
	cmd = make_command_array(token, cmd);
	if (!cmd)
		return (false);
	if (check_builtin(cmd->cmd[0]) >= 0)
		return (true);
	if (cmd->cmd[0])
	{
		if (cmd->cmd[0][0] == '/')
			cmd->pathname = strjoin_with_free("", cmd->cmd[0], NO_FREE);
		else if (cmd->cmd[0][0] == '.')
			cmd->pathname = make_pwd_path(cmd->cmd[0], getenv("PWD"));
		else
			cmd->pathname = make_cmd_and_check_access(cmd->cmd[0],
					path, getenv("PWD"));
		if (cmd->pathname)
			return (true);
	}
	cmd->pathname = strjoin_with_free("", cmd->cmd[0], NO_FREE);
	if (!cmd->pathname)
		return (false);
	return (true);
}

t_cmd	*make_cmd(t_token *token, t_cmd *cmd, char **path)
{
	int	flag;

	flag = 0;
	cmd = NULL;
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
			if (token->next)
				token = token->next;
			break ;
		}
		if (count_array(token) && flag == 0)
		{
			flag++;
			if (!make_path_cmd(token, cmd, path))
				return (free_cmd(cmd), NULL);
		}
		if (token->kind >= RDFILE && token->kind <= WRF_APP)
			open_files(cmd, token);
		if (token->kind == BUILTIN && cmd->status != SYNTAX)
			cmd->status = BUILTIN;
		if (token->kind >= COMMAND && token->kind <= WRF_APP)
			token = token->next;
		else
			break ;
		// if ((token->kind >= COMMAND && token->kind <= WRF_APP) && token->next)
		// 	token = token->next;
	}
	if (cmd-> pathname && access(cmd->pathname, X_OK) != 0 && !set_err_message(cmd, cmd->cmd[0]))
		return (NULL);
	cmd->token = token;
	return (cmd);
}
