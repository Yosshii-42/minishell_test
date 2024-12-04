/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 20:39:15 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/04 19:28:02 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	bool	check_quote_error(char *word)
{
	int		i;
	char	quote;

	i = 0;
	while (word[i])
	{
		if (word[i] == '\'' || word[i] == '"')
		{
			quote = word[i++];
			while (word[i] && word[i] != quote)
				i++;
			if (word[i] != quote)
			{
				ft_printf(2, "syntax error: unmatched quote `%c`\n", quote);
				return (false);
			}
			i++;
		}
		else
			i++;
	}
	return (true);
}

bool	find_syntax_error(t_token *tokenized)
{
	t_kind	prev_kind;

	prev_kind = PIPE;
	while (tokenized)
	{
		if (!check_quote_error(tokenized->word))
			return (false);
		prev_kind = tokenized->kind;
		tokenized = tokenized->next;
	}
	return (true);
}
