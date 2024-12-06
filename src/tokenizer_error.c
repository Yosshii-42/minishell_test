/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 20:39:15 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/06 02:26:36 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	single_roop(char *word, int *i)
{
	int	count_single;
	int	count_double;

	count_single = 1;
	count_double = 0;
	(*i)++;
	while (word[*i])
	{
		if (word[*i] == '\'')
		{
			count_single++;
			break ;
		}
		if (word[*i] == '\"')
			count_double++;
		(*i)++;
	}
	if (count_single == 1 || count_double % 2 == 1)
		return (false);
	return (true);
}

bool	double_roop(char *word, int *i)
{
	int	count_single;
	int	count_double;

	count_single = 0;
	count_double = 1;
	(*i)++;
	while (word[*i])
	{
		if (word[*i] == '\"')
		{
			count_double++;
			break ;
		}
		if (word[*i] == '\'')
			count_single++;
		(*i)++;
	}
	if (count_single % 2 == 1 || count_double == 1)
		return (false);
	return (true);
}

bool	find_syntax_error(char *input)// *tokenized)
{
	int	i;
	i = 0;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (input[i] == '\'')
		{
			if (single_roop(input, &i) == false)
			{
				ft_printf(2, "syntax error: unmatched quote `\'`\n");
				return (false);	
			}
		}
		if (input[i] == '\"')
		{
			if (double_roop(input, &i) == false)
			{
				ft_printf(2, "syntax error: unmatched quote `\"`\n");
				return (false);	
			}
		}
		i++;
	}
	return (true);
}
