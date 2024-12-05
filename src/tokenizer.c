/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:28:45 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/06 02:36:11 by tsururukako      ###   ########.fr       */
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

void	append_quote_token(char *input, t_token **head, t_token *new, int *i)
{
	printf("here\n");
	int		len;
	char	quote;

	len = 0;
	quote = *input;
	if (input[len + 1])
		len++;
	else
		return ;
	while (input[len] && input[len] != quote)
		len++;
	len++;
	new->word = ft_substr(input, 0, len);
	token_lstadd_back(head, new);
	*i += len;
}

void	append_spcial_token(char *input, t_token **head, t_token *new, int *i)
{
	int		len;
	char	special_char;

	len = 0;
	special_char = *input;
	while (input[len] && input[len] == special_char)
		len++;
	new->word = ft_substr(input, 0, len);
	token_lstadd_back(head, new);
	*i += len;
}

void	append_normal_token(char *input, t_token **head, t_token *new, int *i)
{
	int	len;
	
	len = 0;
	while (input[len] && ft_strchr(SPECIAL_CHAR, input[len]) == 0 && !ft_isspace(input[len]))
		len++;
	new->word = ft_substr(input, 0, len);
	token_lstadd_back(head, new);
	*i += len;
}

t_token	*tokenizer(char *input)
{
	t_token	*head;
	t_token	*new;
	int		i;

	head = NULL;
	i = 0;
	// input = space_skip(input);
	while (input[i])
	{
		new = NULL;
		new = (t_token *)safe_malloc(1, sizeof(t_token));
		init_token(new);
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (input[i] && (input[i] == '\'' || input[i] == '\"'))
			append_quote_token(&input[i], &head, new, &i);
		else if (input[i] && ft_strchr(SPECIAL_TOKEN, input[i]))
			append_spcial_token(&input[i], &head, new, &i);
		else if (input[i])
			append_normal_token(&input[i], &head, new, &i);
		i++;
		// if (ft_strchr(SPECIAL_TOKEN, *input))
		// {
		// 	token_len = count_meta_len(input);
		// 	append_token(&input, token_len, &head, new);
		// }
		// else
		// {
		// 	token_len = count_word_len(input, new);
		// 	append_token(&input, token_len, &head, new);
		// }
		// input = space_skip(input);
	}
	return (head);
}
