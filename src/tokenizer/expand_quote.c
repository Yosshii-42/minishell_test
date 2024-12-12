/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_expand_quote.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-11 09:23:11 by yotsurud          #+#    #+#             */
/*   Updated: 2024-12-11 09:23:11 by yotsurud         ###   ########.fr       */
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
		else if (tokenized[i] == '\"')
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
