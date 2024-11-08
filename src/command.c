/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:27:09 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/05 20:49:37 by tsururukako      ###   ########.fr       */
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
		if (cmd->err_msg)
			cmd->err_msg = strjoin_with_free(cmd->err_msg,
				": command not found\n", FREE_S1);
	}
	// if (!cmd->msg)
	// malloc error時の処理
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
int	count_array(t_token *token)
{
	int	count;
	count = 0;
	while (token)
	{
		if (token->kind == COMMAND || token->kind == OPTION)
			count++;
		if (token->next)
			token = token->next;
		else
			break ;
		if (token->kind == PIPE)
			break;
	}
	return (count);
}

int	count_token(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		printf("token = %s\n", token->word);
		count++;
		if (token->next)
			token = token->next;
		else
			break ;
		if (token->kind == PIPE)
			break;
	}
	return (count);
}

static t_cmd	*make_command_array(t_token *token, t_cmd *cmd)
{
	// t_token	*ptr;
	int		array_count;
	int		token_count;
	int		i;
	int		j;

	// ptr = token;
	array_count = count_array(token);
	token_count = count_token(token);

	printf("token_count = %d, array_count = %d\n", token_count, array_count);
	cmd->cmd = (char **)malloc(sizeof(char *) * (array_count + 1));
	if (!(cmd->cmd))
		return (NULL);
	i = -1;
	j = -1;
	while (++i < token_count)
	{
		if (token->kind == COMMAND || token->kind == OPTION)
		{
			cmd->cmd[++j] = ft_strdup(token->word);
			if (!cmd->cmd[j])
				return (free_split(cmd->cmd), NULL);
			printf("cmd = %s\n", cmd->cmd[j]);	
		}
		token = token->next;
	}
	cmd->cmd[array_count] = NULL;
	// cmd->token = ptr;
	return (cmd);
}

static bool	make_path_cmd(t_token *token, t_cmd *cmd, char **path, char *pwd)
{
	cmd = make_command_array(token, cmd);
	if (!cmd)
		return (false);
	if ((cmd->cmd[0]))
	{
		if (cmd->cmd[0][0] == '/')
			cmd->pathname = strjoin_with_free("", cmd->cmd[0], NO_FREE);
		else if (cmd->cmd[0][0] == '.')
			cmd->pathname = make_pwd_path(cmd->cmd[0], pwd);
		else
			cmd->pathname = make_cmd_and_check_access(cmd->cmd[0], path, pwd);
		if (cmd->pathname)
			return (true);
	}
	cmd->pathname = strjoin_with_free("", cmd->cmd[0], NO_FREE);
	if (!cmd->pathname)
		return (false);
	return (true);
}

t_cmd	*make_cmd(t_token *token, t_cmd *cmd, char **path, char *pwd)
{
	printf("make_cmd token = %s\n", token->word);
	int	flag;

	flag = 0;
	cmd = (t_cmd *)malloc(sizeof (t_cmd));
	if (!cmd)
		return (NULL);
	init_cmd(cmd);
	while (token)
	{
		if(token->kind == SYNTAX && (cmd->status = SYNTAX))
			break ;
		if (token->kind == PIPE)
		{
			write(1, "pipe\n", 5);
			if (!make_pipe(cmd))
				return (free_cmd(cmd), NULL);
		 	if (token->next)
				token = token->next;
			printf("after pipe token = %s\n", token->word);
			break ;
		}
		if (flag == 0)
		{
			// write(1, "here\n", 5);
			flag++;
			if (!make_path_cmd(token, cmd, path, pwd))
				return (free_cmd(cmd), NULL);
		}
		else if (token->kind >= RDFILE && token->kind <= WRF_APP)
			open_files(cmd, token);
		else if (token->kind == OPTION || token->kind == COMMAND || (token->kind >= RDFILE && token->kind <= WRF_APP))
		{
			if (token->next)
				token = token->next;
			else
				break ;
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
