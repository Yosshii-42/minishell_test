/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:29:09 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/05 20:30:43 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// static t_env	*change_last_node(t_env *env)
// {
// 	t_env	*ptr;

// 	ptr = env;
// 	env = lstlast(env);
// 	free(env->value);
// 	env->value = ft_strdup("/usr/bin/env");
// 	return (ptr);
// }

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
		lstnew(&start, envp[i]);
	//start = change_last_node(start);
	return (start);
}
