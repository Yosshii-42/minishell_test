/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 20:39:15 by hurabe            #+#    #+#             */
/*   Updated: 2024/11/28 18:19:40 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	bool	check_token_kind(t_kind kind, t_token *token)
{
	if (!(kind == COMMAND || kind == PIPE || kind == LESSTHAN || \
		kind == MORETHAN || kind == HERE_DOC || kind == APPEND || \
		kind == RDFILE || kind == WRF_APP))
	{
		ft_printf(2, "syntax error: unexpected token `%s`\n", token->word);
		return (false);
	}
	return (true);
}

static	bool	check_pipe_redir(t_kind kind, t_token *token, t_kind prev_kind)
{
	if (prev_kind == PIPE && kind == PIPE)
	{
		ft_printf(2, "syntax error: `|` unexpected\n");
		return (false);
	}
	if (kind == PIPE && !token->next)
	{
		ft_printf(2, "syntax error: `|` at end\n");
		return (false);
	}
	if ((prev_kind == LESSTHAN || prev_kind == MORETHAN) && kind != COMMAND)
	{
		ft_printf(2, "syntax error: `%s` after redir\n", token->word);
		return (false);
	}
	return (true);
}

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
		if (!check_token_kind(tokenized->kind, tokenized))
			return (false);
		if (!check_pipe_redir(tokenized->kind, tokenized, prev_kind))
			return (false);
		if (!check_quote_error(tokenized->word))
			return (false);
		prev_kind = tokenized->kind;
		tokenized = tokenized->next;
	}
	return (true);
}
