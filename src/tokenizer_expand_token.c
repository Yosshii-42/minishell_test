/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_expand_token.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:44:28 by hurabe            #+#    #+#             */
/*   Updated: 2024/11/28 17:25:01 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	append_char(char **str, char c)
{
	char	*new;
	char	join[2];

	join[0] = c;
	join[1] = '\0';
	new = ft_strjoin(*str, join);
	if (!new)
		return (false);
	free(*str);
	*str = new;
	return (true);
}

bool	expand_token(t_token *tokenized)
{
	while (tokenized)
	{
		if (tokenized->kind == COMMAND || tokenized->kind == OPTION)
		{
			if (!expand_dollar(tokenized))
				return (false);
			if (!expand_quote(tokenized))
				return (false);
		}
	tokenized = tokenized->next;
	}
	return (true);
}
