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

void	set_str_to_path_and_cmd(t_cmd *cmd, char *line)
{
	int		len;
	int		i;

	len = ft_strlen(line);
	cmd->pathname = safe_malloc(len + 1, sizeof(char));
	cmd->cmd = safe_malloc(2, sizeof(char *));
	cmd->cmd[0] = safe_malloc(len + 1, sizeof(char));
	i = -1;
	while (++i < len)
	{
		cmd->pathname[i] = line[i];
		cmd->cmd[0][i] = line[i];
	}
	cmd->pathname[i] = '\0';
	cmd->cmd[0][i] = '\0';
	cmd->cmd[1] = NULL;
}

void	make_path_and_cmd(char *line, t_cmd *cmd, char **path, char *pwd)
{
	// if (!str)
	// 	return (NULL);
	if (!ft_strlen(line))// || line[0] == ' ' || line[ft_strlen(line) - 1] == ' ')
	{
		set_str_to_path_and_cmd(cmd, line);
		return ;
	}
	cmd->cmd = ft_split(line, ' ');

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

t_cmd	*make_cmd(char *cmd_line, t_env *env)
{
	t_cmd	*cmd;
	char	**path;
	// t_env	*tmp;

	cmd = NULL;
	path = NULL;
	cmd = safe_malloc(1, sizeof(t_cmd));
	init_cmd(cmd);
    path = NULL;
	path = ft_split(getenv_str(env, "PATH"), ':');
	// if (i)
		safe_pipe(cmd);
	// if (index == 0)
	// 	open_read_file(cmd, info, argv[info->infile_index]);
	// if (index == info->cmd_count - 1)
	// 	open_write_file(cmd, info, argv[info->outfile_index]);
	// cmd->pathname = make_path_and_cmd(argv[info->cmd_start + index], cmd, env);
	make_path_and_cmd(cmd_line, cmd, path, getenv_str(env, "PWD"));
	ft_free_split(path);
	if (access(cmd->pathname, X_OK) != 0)
		set_err_message(cmd, cmd->cmd[0]);
	return (cmd);
}
