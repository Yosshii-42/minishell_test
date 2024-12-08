/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 07:19:29 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/07 21:56:53 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	lstadd_front(t_env **start, t_env *new)
{
	if (new)
	{
		new->next = *start;
		*start = new;
	}
}

int	lstnew_export(t_env **start, char *env)
{
	t_env	*new;
	int		len;

	new = (t_env *)safe_malloc(1, sizeof(t_env));
	len = 0;
	if (ft_strchr(env, '='))
		len = strchr_len(env, '=');
	else
		len = ft_strlen(env);
	new->key = (char *)safe_malloc(len + 1, sizeof(char));
	ft_strlcpy(new->key, env, len + 1);
	if (ft_strchr(env, '='))
		new->value = ft_strdup((ft_strchr(env, '=') + 1));
	new->next = NULL;
	lstadd_front(start, new);
	return (TRUE);
}
