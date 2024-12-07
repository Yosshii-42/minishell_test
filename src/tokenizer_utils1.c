/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:24:10 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/07 15:54:03 by yotsurud         ###   ########.fr       */
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

t_token	*token_lstlast(t_token *head)
{
	t_token	*ptr;

	ptr = head;
	if (!ptr)
		return (NULL);
	while (ptr->next)
		ptr = ptr->next;
	return (ptr);
}

void	token_lstadd_back(t_token **head, t_token *new)
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
