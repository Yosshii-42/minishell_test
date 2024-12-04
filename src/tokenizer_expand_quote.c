/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_expand_quote.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 23:37:36 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/04 19:31:23 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*single_roop(char *new, char *tokenized, int *i, int *single_flag)
{
	(*single_flag)++;
	(*i)++;
	while (tokenized[*i] && tokenized[*i] != '\'')
	{
		new = ft_strjoin_one(new, tokenized[*i]);
		(*i)++;
	}
	if (tokenized[*i] == '\'')
		(*single_flag)++;
	return (new);
}	

static char	*double_roop(char *new, char *tokenized, int *i, int *double_flag)
{
	(*double_flag)++;
	(*i)++;
	while (tokenized[*i] && tokenized[*i] != '\"')
	{
		new = ft_strjoin_one(new, tokenized[*i]);
		(*i)++;
	}
	if (tokenized[*i] == '\"')
		(*double_flag)++;
	return (new);
}

char	*expand_quote(char *tokenized, t_token *token)
{
	int		i;
	int		single_flag;
	int		double_flag;
	char	*new;

	init_variables(&i, &single_flag, &double_flag);
	new = ft_strdup("");
	while (tokenized[i])
	{
		if ((single_flag + double_flag) % 2 == 0 && tokenized[i] == '\'')
			new = single_roop(new, tokenized, &i, &single_flag);
		else if ((single_flag + double_flag) % 2 == 0 && tokenized[i] == '\"')
			new = double_roop(new, tokenized, &i, &double_flag);
		else if (!(tokenized[i] == '\'' || tokenized[i] == '\"'))
			new = ft_strjoin_one(new, tokenized[i]);
		i++;
	}
	free(token->word);
	token->word = new;
	return (new);
}
