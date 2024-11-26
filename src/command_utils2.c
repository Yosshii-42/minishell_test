/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 06:27:24 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/27 00:47:39 by tsururukako      ###   ########.fr       */
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
			break;
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
			break;
		token = token->next;
	}
	return (count);
}
