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
	char	*tmp;

	len = 0;
	quote = *input;
	if (input[len + 1])
		len++;
	else
		return ;
	while (input[len] && input[len] != quote)
		len++;
	len++;
	tmp = strjoin_with_free(new->word, ft_substr(input, 0, len), FREE_ALL);
	new->word = tmp;
		*i += len;
	new->is_quoted = true;
}

void	append_spcial_token(char *input, t_token *new, int *i)
{
	int		len;
	char	special_char;
	char	*tmp;

	len = 0;
	special_char = *input;
	if (input[len + 1])
		len++;
	else
		return ;
	while (input[len] && input[len] == special_char)
		len++;
	tmp = strjoin_with_free(new->word, ft_substr(input, 0, len), FREE_ALL);
	new->word = tmp;
	*i += len;
}

void	append_normal_token(char *input, t_token *new, int *i)
{
	int		len;
	char	*tmp;

	len = 0;
	if (input[len + 1])
		len++;
	else
		return ;
	while (input[len] && ft_strchr(SPECIAL_CHAR, input[len]) == 0 && !ft_isspace(input[len]))
		len++;
	tmp = strjoin_with_free(new->word, ft_substr(input, 0, len), FREE_ALL);
	new->word = tmp;
	*i += len;
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
		while (input[i])
		{
			if (input[i] && ft_strchr(SPECIAL_TOKEN, input[i]))
			{
				append_spcial_token(&input[i], new, &i);
				break ;
			}
			else if (input[i] && (input[i] == '\'' || input[i] == '\"'))
				append_quote_token(&input[i], new, &i);
			else if (input[i])
				append_normal_token(&input[i], new, &i);
			if (input[i] && ft_isspace(input[i]))
				break ;
		}
		token_lstadd_back(&head, new);
	}
	return (head);
}
