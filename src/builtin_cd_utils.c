/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 22:04:13 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/06 22:06:56 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_arguments(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

char	*skip_spaces(char *str)
{
	while (*str && ft_isspace(*))
}
