/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:49:39 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/26 00:51:07 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	set_err_message(t_cmd *cmd, char *str, char *err_str)
{
	if (cmd->err_msg)
		return (true);
	cmd->err_msg = NULL;
	if (cmd->status == SYNTAX)
		cmd->err_msg = ft_strdup("bash: syntax error\n");
	else if (!str || !*str)
		cmd->err_msg = strjoin_with_free("", "Command ' ' not found\n", NO_FREE);
	else
	{
		cmd->err_msg = strjoin_with_free("bash: ", str, NO_FREE);
		if (cmd->err_msg)
			cmd->err_msg = strjoin_with_free(cmd->err_msg,
					": ", FREE_S1);
		if (cmd->err_msg && (cmd->cmd[0][0] == '.' || cmd->cmd[0][0] == '/'))
			cmd->err_msg = strjoin_with_free(cmd->err_msg, err_str, FREE_S1);
		else if (cmd->err_msg)
			cmd->err_msg = strjoin_with_free(cmd->err_msg, "command not found", FREE_S1);
		if (cmd->err_msg)
			cmd->err_msg = strjoin_with_free(cmd->err_msg, "\n", FREE_S1);
	}
	if (!cmd->err_msg)
		exit((ft_printf(2, "malloc: %s\n", strerror(errno)), EXIT_FAILURE));
	return (true);
}

static char	*make_cmd_and_check_access(t_cmd *cmd, char *pwd)
{
	char	*str;
	int		i;
	
	str = NULL;
	if (!cmd->path)
		return (make_pwd_path(cmd->cmd[0], pwd));
	i = -1;
	while (cmd->path[++i])
	{
		str = strjoin_with_free(cmd->path[i], "/", NO_FREE);
		if (str)
			str = strjoin_with_free(str, cmd->cmd[0], FREE_S1);
		// if (!str)
		// 	return (NULL);//後で検討
		if (!access(str, X_OK))
			return (str);
		free(str);
		str = NULL;
	}
	return (strjoin_with_free("x", cmd->cmd[0], NO_FREE));
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
		exit((ft_printf(2, "malloc: %s\n", strerror(errno)), EXIT_FAILURE));
	i = -1;
	j = -1;
	while (++i < token_count)
	{
		if (token->kind == BUILTIN || token->kind == COMMAND || token->kind == OPTION)
		{
			cmd->cmd[++j] = ft_strdup(token->word);
			if (!cmd->cmd[j])
				exit((ft_printf(2, "malloc: %s\n", strerror(errno)), EXIT_FAILURE));
		}
		token = token->next;
	}
	cmd->cmd[array_count] = NULL;
	return (cmd);
}

static bool	make_path_cmd(t_token *token, t_cmd *cmd)
{
	cmd = make_command_array(token, cmd);
	if (check_builtin(cmd->cmd[0]) >= 0)
		return (true);
	if (cmd->cmd[0])
	{
		if (cmd->cmd[0][0] == '/')
			cmd->pathname = strjoin_with_free("", cmd->cmd[0], NO_FREE);
		else if (cmd->cmd[0][0] == '.')
			cmd->pathname = make_pwd_path(cmd->cmd[0], getenv("PWD"));
		else
			cmd->pathname = make_cmd_and_check_access(cmd, getenv("PWD"));
		if (cmd->pathname)
			return (true);
	}
	cmd->pathname = strjoin_with_free("", cmd->cmd[0], NO_FREE);
	return (true);
}

t_cmd	*command_return(t_cmd *cmd, t_token *token)
{
	if (cmd->pathname && access(cmd->pathname, X_OK) != 0
		&& !set_err_message(cmd, cmd->cmd[0], strerror(errno)))
	return (NULL);
	cmd->token = token;
	return (cmd);
}

t_cmd	*malloc_command(t_cmd *cmd)
{
	cmd = NULL;
	cmd = (t_cmd *)malloc(sizeof (t_cmd));
	if (!cmd)
		exit((ft_printf(2, "malloc: %s\n", strerror(errno)), EXIT_FAILURE));
	return (cmd);
}

t_cmd	*make_cmd(t_token *token, t_cmd *cmd, int command_flag)
{
	cmd = malloc_command(cmd);
	init_cmd(cmd);
	while (token)
	{
		if (token->kind == SYNTAX)
			return (cmd->status = SYNTAX, command_return(cmd, token));
		if (token->kind == PIPE && !make_pipe(cmd))
			return (free_cmd(cmd), free_token(set_token(GET, NULL)), NULL);
		if (token->kind == PIPE && token->next)
			return (token = token->next, command_return(cmd, token));
		if (count_array(token) && command_flag == 0)
		{
			command_flag++;
			make_path_cmd(token, cmd);
		}
		if (token->kind >= RDFILE && token->kind <= WRF_APP)
			open_files(cmd, token);
		if (token->kind == BUILTIN && cmd->status != SYNTAX)
			cmd->status = BUILTIN;
		if (token->kind >= COMMAND && token->kind <= WRF_APP)
			token = token->next;
		else
			break ;
	}
	return (command_return(cmd, token));
}
