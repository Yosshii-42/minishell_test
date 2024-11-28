/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_expand_quote.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 23:37:36 by hurabe            #+#    #+#             */
/*   Updated: 2024/11/28 19:11:36 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	expand_quote(t_token *tokenized)
{
	int		i;
	char	quote;
	char	*new;

	i = 0;
	quote = 0;
	new = ft_strdup("");
	if (!new)
		return (false);
	while (tokenized->word[i])
	{
		if (is_quote(tokenized->word[i]) && quote == 0)
			quote = tokenized->word[i++];
		else if (tokenized->word[i] == quote)
		{
			quote = 0;
			i++;
		}
		else
			append_char(&new, tokenized->word[i++]);
	}
	free(tokenized->word);
	tokenized->word = new;
	return (true);
}
