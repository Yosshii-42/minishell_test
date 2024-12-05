/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_expand_quote.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 23:37:36 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/05 15:24:50 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_quote(char *tokenized, t_token *token)
{
	int		i;
	char	*new;

	i = 0;
	new = ft_strdup("");
	while (tokenized[i])
	{
		if (tokenized[i] == '\'')
		{
			i++;
			while (tokenized[i] && tokenized[i] != '\'')
				new = ft_strjoin_one(new, tokenized[i++]);
		}
		if (tokenized[i] == '\"')
		{
			i++;
			while (tokenized[i] && tokenized[i] != '\"')
				new = ft_strjoin_one(new, tokenized[i++]);
		}
		else if (!(tokenized[i] == '\'' || tokenized[i] == '\"'))
			new = ft_strjoin_one(new, tokenized[i]);
		i++;
	}
	return (free(token->word), new);
}
