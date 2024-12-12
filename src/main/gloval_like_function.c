/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_gloval_like_function.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 06:20:36 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/28 17:12:07 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	end_status(int type, int end_status)
{
	static int	status;

	if (type == SET)
		status = end_status;
	return (status);
}

t_env	*set_env(int type, t_env *new_env)
{
	static t_env	*env;

	if (type == SET)
		env = new_env;
	return (env);
}

t_token	*set_token(int type, t_token *new_token)
{
	static t_token	*token;

	if (type == SET)
		token = new_token;
	return (token);
}
