/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:28:45 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/04 19:39:34 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_token(t_token *token)
{
	token->word = NULL;
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

void	append_token(char **input, int token_len, t_token **head, t_token *new)
{
	new->next = NULL;
	new->word = ft_substr(*input, 0, token_len);
	token_lstadd_back(head, new);
	*input += token_len;
}

t_token	*tokenizer(char *input)
{
	t_token	*head;
	t_token	*new;
	int		token_len;

	head = NULL;
	input = space_skip(input);
	while (*input)
	{
		new = NULL;
		new = (t_token *)safe_malloc(1, sizeof(t_token));
		init_token(new);
		if (ft_strchr(SPECIAL_TOKEN, *input))
		{
			token_len = count_meta_len(input);
			append_token(&input, token_len, &head, new);
		}
		else
		{
			token_len = count_word_len(input, new);
			append_token(&input, token_len, &head, new);
		}
		input = space_skip(input);
	}
	return (head);
}
