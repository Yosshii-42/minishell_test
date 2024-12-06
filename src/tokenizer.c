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

char	*append_quote_token(char *input, t_token *new, int *i)
{
	int		len;
	char	quote;
	char	*tmp;
	char	*sub_str;

	len = 0;
	quote = *input;
	if (input[len + 1])
		len++;
	else
		return (new->word);
	while (input[len] && input[len] != quote)
		len++;
	len++;
	sub_str = ft_substr(input, 0, len);
	tmp = strjoin_with_free(new->word, sub_str, FREE_S1);
	free(sub_str);
		*i += len - 1;
	new->is_quoted = true;
	return (tmp);
}

char	*append_spcial_token(char *input, t_token *new, int *i)
{
	int		len;
	char	special_char;
	char	*tmp;
	char	*sub_str;

	len = 0;
	special_char = *input;
	if (input[len + 1])
		len++;
	else
		return (new->word);
	while (input[len] && input[len] == special_char)
		len++;
	sub_str = ft_substr(input, 0, len);
	tmp = strjoin_with_free(new->word, sub_str, FREE_S1);
	free(sub_str);
	*i += len;
	return (tmp);
}

char	*append_normal_token(char *input, t_token *new, int *i)
{
	int		len;
	char	*tmp;
	char	*sub_str;

	len = 0;
	while (input[len] && ft_strchr(SPECIAL_CHAR, input[len]) == 0
		&& !ft_isspace(input[len]))
		len++;
	sub_str = ft_substr(input, 0, len);
	tmp = strjoin_with_free(new->word, sub_str, FREE_S1);
	free(sub_str);
	*i += len;
	return (tmp);
}

char	*append_roop(char *input, int *i, t_token *new)
{
	int	j;

	j = 0;
	while (input[j])
	{
		if (ft_strchr(SPECIAL_TOKEN, input[j]))
		{
			new->word = append_spcial_token(&input[j], new, &j);
			break ;
		}
		else if (input[j] == '\'' || input[j] == '\"')
			new->word = append_quote_token(&input[j], new, &j);
		else
			new->word = append_normal_token(&input[j], new, &j);
		if (!input[j] || (input[j] && ft_isspace(input[j])))
			break ;
		else
			j++;
	}
	*i += j;
	return (new->word);
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
		new->word = append_roop(&input[i], &i, new);
		token_lstadd_back(&head, new);
	}
	return (head);
}
