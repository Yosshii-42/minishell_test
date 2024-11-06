/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-06 05:45:20 by yotsurud          #+#    #+#             */
/*   Updated: 2024-11-06 05:45:20 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool    builtin_exit(char *line)
{
    int result;

    result = ft_memcmp(line, "exit", 5);
    if (result == 0 || result == 9 || result == 10 || result == 32)
        return (true);
    else
        return (false);
}

