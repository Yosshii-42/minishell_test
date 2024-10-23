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
		cmd->err_msg = strjoin_with_free(cmd->err_msg, ": command not found\n", FREE_S1);
	}
}

static char	*check_access(char *command, char **path, char *pwd)
{
	char	*str;
	int	i;

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

static char **make_command_array(t_token *token)
{
	char	**cmd;
	t_token	*ptr;
	int		count;
	int		i;

	ptr = token;
	count = 0;
	while (ptr->kind == COMMAND || ptr->kind == ARGUMENT)
	{
		count++;
		if (ptr->next)
			ptr = ptr->next;
		else
			break;
	}
	cmd = (char **)malloc(sizeof(char *) * (count + 1));
	if (!cmd)
		return (NULL);
	i = -1;
	while (++i < count)
	{
		cmd[i] = ft_strdup(token->word);
		token = token->next;
		if (!cmd[i])
			return (ft_free_split(cmd), NULL);
	}
	cmd[i] = NULL;
	return (cmd);
}

static void	make_path_and_cmd(t_token *token, t_cmd *cmd, t_env *env, char **path)
{
	// char	**path;
	char	*pwd;

	// path = ft_split(getenv_str(env, "PATH"), ':');
	pwd = getenv_str(env, "PWD");
	cmd->cmd = make_command_array(token);
	if (!cmd)
		exit (1); //TODO error_exit
	if ((cmd->cmd[0]))
	{
		if (cmd->cmd[0][0] == '/')
			cmd->pathname = strjoin_with_free("", cmd->cmd[0], NO_FREE);
		else if (cmd->cmd[0][0] == '.')
			cmd->pathname = make_pwd_path(cmd->cmd[0], pwd);
		else
			cmd->pathname = check_access(cmd->cmd[0], path, pwd);
		if (cmd->pathname)
			return ;
	}
	cmd->pathname = strjoin_with_free("", cmd->cmd[0], NO_FREE);
}

t_cmd	*make_cmd(t_token *token, t_cmd *cmd, t_env *env)
{
	char	**path;
	t_token	*ptr;

	path = NULL;
	cmd = (t_cmd *)malloc(sizeof (t_cmd));
	if (!cmd)
		return (NULL);
	init_cmd(cmd);
    path = NULL;
	path = ft_split(getenv_str(env, "PATH"), ':');
	ptr = token;
	while (ptr)
	{
		if (ptr->kind == PIPE)
		{
			safe_pipe(cmd);
			ptr = ptr->next;
			break;
		}
		else if (ptr->kind == RDFILE || ptr->kind == LIMITTER)
			open_read_file(cmd, ptr);
		else if (ptr->kind == WRFILE || ptr->kind == WRFILE_APP)
			open_write_file(cmd, ptr);
		else if (ptr->kind == COMMAND)
		{
			make_path_and_cmd(ptr, cmd, env, path);
			while (ptr->next)
			{
				if (ptr->next->kind == ARGUMENT)
					ptr = ptr->next;
				else
					break;
			}
		}
		// if (token->kind == END)
		// 	break;
		if (ptr->next)
			ptr = ptr->next;
		else
			break;
	}
	ft_free_split(path);
	if (access(cmd->pathname, X_OK) != 0)
		set_err_message(cmd, cmd->cmd[0]);
	return (cmd);
}
