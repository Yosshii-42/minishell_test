/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-11 09:18:50 by yotsurud          #+#    #+#             */
/*   Updated: 2024-12-11 09:18:50 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_err_message(t_cmd *cmd, char *str, char *err_str)
{
	if (cmd->err_msg)
		return ;
	cmd->err_msg = NULL;
	if (cmd->status == SYNTAX)
		cmd->err_msg = ft_strdup("bash: syntax error\n");
	else if (!str || !*str)
		cmd->err_msg = ft_strdup("Command ' ' not found\n");
	else
	{
		cmd->err_msg = strjoin_with_free(str, ": ", NO_FREE);
		if (cmd->err_msg && (cmd->cmd[0][0] == '.' || cmd->cmd[0][0] == '/'))
			cmd->err_msg = strjoin_with_free(cmd->err_msg, err_str, FREE_S1);
		else if (cmd->err_msg)
			cmd->err_msg = strjoin_with_free
				(cmd->err_msg, "command not found", FREE_S1);
		cmd->err_msg = strjoin_with_free(cmd->err_msg, "\n", FREE_S1);
	}
}

static char	*make_cmd_and_check_access(t_cmd *cmd)
{
	char	*str;
	int		i;

	str = NULL;
	if (!cmd->path)
		return (make_pwd_path(cmd));
	i = -1;
	while (cmd->path[++i])
	{
		str = strjoin_with_free(cmd->path[i], "/", NO_FREE);
		str = strjoin_with_free(str, cmd->cmd[0], FREE_S1);
		if (!access(str, X_OK))
			return (str);
		free(str);
		str = NULL;
	}
	return (NULL);
}

static t_cmd	*make_command_array(t_token *token, t_cmd *cmd)
{
	int		array_count;
	int		token_count;
	int		i;
	int		j;

	array_count = count_array(token);
	token_count = count_token(token);
	cmd->cmd = (char **)safe_malloc(array_count + 1, sizeof(char *));
	i = -1;
	j = -1;
	while (++i < token_count)
	{
		if (token->kind == BUILTIN || token->kind == COMMAND
			|| token->kind == OPTION)
			cmd->cmd[++j] = ft_strdup(token->word);
		token = token->next;
	}
	cmd->cmd[array_count] = NULL;
	return (cmd);
}

static bool	make_path_cmd(t_token *token, t_cmd *cmd, int *command_flag)
{
	(*command_flag)++;
	if (!token->word || !token->word[0])
		return (true);
	cmd = make_command_array(token, cmd);
	if (!cmd)
		return (true);
	if (check_builtin(cmd->cmd[0]) >= 0)
		return (true);
	if (cmd->cmd[0])
	{
		if (cmd->cmd[0][0] == '/')
			cmd->pathname = ft_strdup(cmd->cmd[0]);
		else if (cmd->cmd[0][0] == '.')
			cmd->pathname = make_pwd_path(cmd);
		else
			cmd->pathname = make_cmd_and_check_access(cmd);
		if (!cmd->pathname && cmd->cmd[0])
			cmd->pathname = ft_strdup(cmd->cmd[0]);
	}
	return (true);
}

t_cmd	*make_cmd(t_token *token, t_cmd *cmd, int command_flag)
{
	while (token)
	{
		if (!token->word[0] && token->is_dollar == true)
			token = token->next;
		else
		{
			if (token->kind == SYNTAX || (token->kind == PIPE && !token->next))
				return (cmd->status = SYNTAX, command_return(cmd, token));
			if (token->kind == PIPE)
				make_pipe(cmd);
			if (token->kind == PIPE && token->next)
				return (token = token->next, command_return(cmd, token));
			if (count_array(token) && command_flag == 0)
				make_path_cmd(token, cmd, &command_flag);
			if (token->kind >= RDFILE && token->kind <= WRF_APP)
				open_files(cmd, token);
			if (token->kind == BUILTIN && cmd->status != SYNTAX)
				cmd->status = BUILTIN;
			if ((token->kind >= COMMAND && token->kind <= WRF_APP))
				token = token->next;
			else
				break ;
		}
	}
	return (command_return(cmd, token));
}
