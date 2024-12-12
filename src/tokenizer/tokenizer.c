/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-07 06:06:34 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/07 15:58:41 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	append_quote_token(char *input, t_token *new, int *i)
{
	int		len;
	char	quote;

	len = 1;
	quote = *input;
	while (input[len] && input[len] != quote)
		len++;
	len++;
	new->word = strjoin_with_free(new->word, ft_substr(input, 0, len),
			FREE_ALL);
	*i += len - 1;
	new->is_quoted = true;
}

static void	append_spcial_token(char *input, t_token *new, int *i)
{
	int		len;
	char	special_char;

	len = 0;
	special_char = *input;
	while (input[len] && input[len] == special_char)
		len++;
	new->word = strjoin_with_free(new->word, ft_substr(input, 0, len),
			FREE_ALL);
	*i += len - 1;
}

static void	append_normal_token(char *input, t_token *new, int *i)
{
	int		len;

	len = 0;
	while (input[len] && ft_strchr(SPECIAL_CHAR, input[len]) == 0
		&& !ft_isspace(input[len]))
		len++;
	new->word = strjoin_with_free(new->word, ft_substr(input, 0, len),
			FREE_ALL);
	*i += len - 1;
}

static void	append_roop(char *input, int *i, t_token *new)
{
	int	j;

	j = 0;
	while (input[j])
	{
		if (ft_strchr(SPECIAL_TOKEN, input[j]))
		{
			append_spcial_token(&input[j], new, &j);
			j++;
			break ;
		}
		else if (input[j] == '\'' || input[j] == '\"')
			append_quote_token(&input[j], new, &j);
		else
			append_normal_token(&input[j], new, &j);
		j++;
		if (!input[j] || (input[j] && (ft_isspace(input[j])
					|| ft_strchr(SPECIAL_TOKEN, input[j]))))
			break ;
	}
	*i += j;
}

t_token	*tokenizer(char *input)
{
	t_token	*head;
	t_token	*new;
	int		i;

	head = NULL;
	i = 0;
	while (input[i] && ft_isspace(input[i]))
		i++;
	while (input[i])
	{
		new = NULL;
		new = (t_token *)safe_malloc(1, sizeof(t_token));
		init_token(new);
		append_roop(&input[i], &i, new);
		token_lstadd_back(&head, new);
		if (!input[i])
			break ;
		while (input[i] && ft_isspace(input[i]))
			i++;
	}
	return (head);
}
