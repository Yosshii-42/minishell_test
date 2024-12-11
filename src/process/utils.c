/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-11 09:21:51 by yotsurud          #+#    #+#             */
/*   Updated: 2024-12-11 09:21:51 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cmd_count(t_token *token)
{
	int		count;
	t_token	*ptr;

	count = 0;
	ptr = token;
	if (!token)
		return (0);
	while (ptr)
	{
		if (*(ptr->word) == '|')
			count++;
		ptr = ptr->next;
	}
	return (count + 1);
}

int	pipe_count(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		if (*(token->word) == '|' )
		{
			count++;
			break ;
		}
		token = token->next;
	}
	return (count);
}

int	make_fork(pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
		exit((ft_printf(2, "fork: %s\n", strerror(errno)), EXIT_FAILURE));
	return (TRUE);
}

void	close_fds(t_cmd *cmd)
{
	if (cmd->readfd > 0)
		close(cmd->readfd);
	if (cmd->writefd > 0)
		close(cmd->writefd);
	if (cmd->pp[0] > 0)
		close(cmd->pp[0]);
	if (cmd->pp[1] > 0)
		close(cmd->pp[1]);
}

char	**make_env_array(void)
{
	char	**env_lst;
	t_env	*env;
	int		count;

	env = set_env(GET, NULL);
	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	env_lst = (char **)safe_malloc(count + 1, sizeof(char *));
	env = set_env(GET, NULL);
	count = 0;
	while (env)
	{
		env_lst[count] = strjoin_with_free(env->key, "=", NO_FREE);
		env_lst[count] = strjoin_with_free(env_lst[count], env->value, FREE_S1);
		env = env->next;
		count++;
	}
	env_lst[count] = NULL;
	return (env_lst);
}
