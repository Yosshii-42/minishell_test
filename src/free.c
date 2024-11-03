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
		if (env->next)
		{
			env = env->next;
			free(env->pre);
		}
		else
		{
			free(env);
			break;
		}
	}
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split[0] || !split)
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
