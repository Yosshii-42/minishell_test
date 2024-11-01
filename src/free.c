#include "../minishell.h"

void	free_env(t_env *env)
{
	while (env)
	{
		if (env->key)
		{
			free(env->key);
			env->key = NULL;
		}
		if (env->value)
		{
			free(env->value);
			env->value = NULL;
		}
		free(env);
		if (env->next)
			env = env->next;
	}
}

void	free_env_and_exit(t_env *env)
{
	free_env(env);
	exit(EXIT_FAILURE);
}

// void	ft_free(char **str, int i)
// {
// 	char	**tmp;

// 	tmp = str;
// 	if (!str)
// 		return ;
// 	while (tmp[i])
// 		free(tmp[i++]);
// 	if (str)
// 		free(str);
// }

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split || !split[0])
		return ;
	while (split[i])
		free(split[i++]);
	free(split);
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd->pathname)
		free(cmd->pathname);
	if (cmd->cmd)
		free_split(cmd->cmd);
	if (cmd->err_msg)
		free(cmd->err_msg);
	free(cmd);
}
