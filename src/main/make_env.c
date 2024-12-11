/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-11 09:21:16 by yotsurud          #+#    #+#             */
/*   Updated: 2024-12-11 09:21:16 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*lstlast(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	lstadd_back(t_env **start, t_env *new)
{
	t_env	*ptr;

	ptr = NULL;
	if (!*start)
	{
		*start = new;
		return ;
	}	
	ptr = lstlast(*start);
	ptr->next = new;
}

int	lstnew(t_env **start, char *env)
{
	t_env	*new;
	int		len;

	new = (t_env *)safe_malloc(1, sizeof(t_env));
	len = 0;
	len = strchr_len(env, '=');
	new->key = (char *)safe_malloc(len + 1, sizeof(char));
	if (!new->key)
		return (free(new), FALSE);
	ft_strlcpy(new->key, env, len + 1);
	new->value = ft_strdup((ft_strchr(env, '=') + 1));
	new->next = NULL;
	lstadd_back(start, new);
	return (TRUE);
}

t_env	*set_no_envp(t_env **start, int i)
{
	static char	*env_key[] = {"PWD", "SHLVL", "_", NULL};
	static char	*env_value[] = {NULL, "1", "/usr/bin/env", NULL};
	char		*pwd;
	t_env		*new;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (perror("getcwd"), NULL);
	env_value[0] = ft_strdup((char *)pwd);
	free(pwd);
	while (env_key[++i])
	{
		new = NULL;
		new = (t_env *)safe_malloc(1, sizeof(t_env));
		new->key = ft_strdup(env_key[i]);
		new->value = ft_strdup(env_value[i]);
		new->next = NULL;
		lstadd_back(start, new);
	}
	free(env_value[0]);
	return (*start);
}

t_env	*make_env(int argc, char **argv, char **envp)
{
	t_env	*start;
	t_env	*tmp;
	int		i;

	if (argc == 0 || !argv[0])
		exit(EXIT_FAILURE);
	i = -1;
	start = NULL;
	if (!(*envp))
		return (set_no_envp(&start, i));
	while (envp[++i])
		lstnew(&start, envp[i]);
	tmp = start;
	while (tmp)
	{
		if (ft_memcmp(tmp->key, "_", 2) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup("/usr/bin/env");
		}
		tmp = tmp->next;
	}
	return (start);
}
