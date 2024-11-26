/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:29:09 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/25 23:05:04 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_env	*lstlast(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

static t_env	*change_last_node(t_env *env)
{
	char	*tmp;
	t_env	*ptr;

	ptr = env;
	env = lstlast(env);
	tmp = env->value;
	env->value = ft_strdup("/usr/bin/env");
	free(tmp);
	if (!env->value)
	{
		ft_printf(2, "bash: malloc: %s\n", strerror(errno));
		return (free_env(set_env(GET, NULL)), NULL);
	}
	return (ptr);
}

void	lstadd_back(t_env **start, t_env *new)
{
	t_env	*ptr;

	ptr = NULL;
	if (!*start)
	{
		*start = new;
		new->pre = NULL;
		return ;
	}	
	ptr = lstlast(*start);
	ptr->next = new;
	new->pre = ptr;
}

static int	lstnew(t_env **start, char *env)
{
	t_env	*new;
	int		len;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (FALSE);
	len = 0;
	len = strchr_len(env, '=');
	new->key = (char *)malloc((len + 1) * sizeof(char));
	if (!new->key)
		return (free(new), FALSE);
	ft_strlcpy(new->key, env, len + 1);
	new->value = ft_strdup((ft_strchr(env, '=') + 1));
	if (!new->value)
		return (free(new->key), free(new), FALSE);
	new->next = NULL;
	lstadd_back(start, new);
	return (TRUE);
}

t_env	*make_env(int argc, char **argv, char **envp)
{
	t_env	*start;
	int		i;

	if (argc == 0 || !argv[0])
		exit(EXIT_FAILURE);
	i = -1;
	start = NULL;
	while (envp[++i])
	{
		if (!lstnew(&start, envp[i]))
			return (free_env(set_env(GET, NULL)), NULL);
	}
	start = change_last_node(start);
	return (start);
}
