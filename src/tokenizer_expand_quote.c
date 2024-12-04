/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_expand_quote.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 23:37:36 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/04 19:07:57 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*single_roop(char *new, char *tokenized, int *i, int *single_flag)
{
	(*i)++;
	while (tokenized[*i] && tokenized[*i] != '\'')
	{
		new = ft_strjoin_one(new, tokenized[*i]);
		printf("sigle roop c = %c, i = %d\n", tokenized[*i], *i);
		(*i)++;
	}
	if (tokenized[*i] == '\'')
	{
		(*single_flag)++;
		//(*i)++;
	}
	printf("end of sigle roop c = %c, i = %d\n", tokenized[*i], *i);
	return (new);
}	


static char	*double_roop(char *new, char *tokenized, int *i, int *double_flag)
{		
	(*i)++;
	while (tokenized[*i] && tokenized[*i] != '\"')
	{
		new = ft_strjoin_one(new, tokenized[*i]);
		printf("double roop c = %c, i = %d\n", tokenized[*i], *i);
		(*i)++;
	}
	if (tokenized[*i] == '\"')
	{
		(*double_flag)++;
		//(*i)++;
	}
	printf("end of double roop c = %c, i = %d\n", tokenized[*i], *i);
	return (new);
}

char	*expand_quote(char *tokenized, t_token *token)
{
	int		i;
	int		single_flag;
	int		double_flag;
	char	*new;

	i = 0;
	single_flag = 0;
	double_flag = 0;
	new = ft_strdup("");
	while (tokenized[i])
	{
		if (single_flag % 2 == 0 && double_flag % 2 == 0 && tokenized[i] == '\'')
		{
			single_flag++;
			new = single_roop(new, tokenized, &i, &single_flag);
		}
		else if (single_flag % 2 == 0 && double_flag % 2 == 0 && tokenized[i] == '\"')
		{
			double_flag++;
			new = double_roop(new, tokenized, &i, &double_flag);
		}
		else if (!(tokenized[i] == '\'' || tokenized[i] == '\"'))
			new = ft_strjoin_one(new, tokenized[i]);
		i++;
	}
	free(token->word);
	token->word = new;
	return (new);
}
