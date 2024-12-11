/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-11 09:20:54 by yotsurud          #+#    #+#             */
/*   Updated: 2024-12-11 09:20:54 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	limitter_warning(int count, char *eof)
{
	ft_printf(2, "\nbash: warning: here-document delimited at ");
	ft_printf(2, "line%d by end-of-file (wanted `%s')\n", count, eof);
}

int	count_array(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		if (token->kind == COMMAND || token->kind == BUILTIN
			|| token->kind == OPTION)
			count++;
		if (token->kind == PIPE)
			break ;
		token = token->next;
	}
	return (count);
}

int	count_token(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		count++;
		if (token->kind == PIPE)
			break ;
		token = token->next;
	}
	return (count);
}
