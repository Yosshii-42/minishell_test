/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-11 09:19:43 by yotsurud          #+#    #+#             */
/*   Updated: 2024-12-11 09:19:43 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	lstadd_back(start, new);
	return (TRUE);
}
