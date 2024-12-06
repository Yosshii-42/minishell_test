/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:28:45 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/07 00:17:19 by tsururukako      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_token(t_token *token)
{
	token->word = ft_strdup("");
	token->kind = -1;
	token->status = -1;
	token->is_dollar = false;
	token->is_quoted = false;
	token->is_double_quoted = false;
	token->next = NULL;
	token->pre = NULL;
}

static	t_token	*token_lstlast(t_token *head)
{
	t_token	*ptr;

	ptr = head;
	if (!ptr)
		return (NULL);
	while (ptr->next)
		ptr = ptr->next;
	return (ptr);
}

static	void	token_lstadd_back(t_token **head, t_token *new)
{
	t_token	*ptr;

	ptr = NULL;
	if (!*head)
	{
		*head = new;
		new->pre = NULL;
		return ;
	}	
	ptr = token_lstlast(*head);
	ptr->next = new;
	new->pre = ptr;
}

void	append_quote_token(char *input, t_token *new, int *i)
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

void	append_spcial_token(char *input, t_token *new, int *i)
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

void	append_normal_token(char *input, t_token *new, int *i)
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

void	append_roop(char *input, int *i, t_token *new)
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
			|| ft_strchr(SPECIAL_TOKEN, input[j])
			|| input[j] == '\'' || input[j] == '\"')))
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
	while (input[i])
	{
		new = NULL;
		new = (t_token *)safe_malloc(1, sizeof(t_token));
		init_token(new);
		while (input[i] && ft_isspace(input[i]))
			i++;
		append_roop(&input[i], &i, new);
		token_lstadd_back(&head, new);
		if (!input[i])
			break ;
	}
	return (head);
}
