/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:29:09 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/03 17:38:56 by hurabe           ###   ########.fr       */
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

static t_env	*change_last_node(t_env *env)
{
	char	*tmp;
	t_env	*ptr;

	ptr = env;
	env = lstlast(env);
	tmp = env->value;
	env->value = ft_strdup("/usr/bin/env");
	free(tmp);
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

int	lstnew(t_env **start, char *env)
{
	t_env	*new;
	int		len;

	new = (t_env *)safe_malloc(1, sizeof(t_env));
	len = 0;
	len = strchr_len(env, '=');
	new->key = (char *)safe_malloc(len + 1, sizeof(char));
	ft_strlcpy(new->key, env, len + 1);
	if (ft_strchr(env, '='))
		new->value = ft_strdup((ft_strchr(env, '=') + 1));
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
		lstnew(&start, envp[i]);
	start = change_last_node(start);
	return (start);
}