/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsururukakou <tsururukakou@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:24:10 by hurabe            #+#    #+#             */
/*   Updated: 2024/12/07 01:06:54 by tsururukako      ###   ########.fr       */
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

// char	*space_skip(char *input)
// {
// 	while (*input && ft_isspace(*input))
// 		input++;
// 	return (input);
// }

// int	count_word_len(char *input, t_token *new)
// {
// 	int	len;

// 	len = 0;
// 	while (*(input + len) && !ft_isspace(*(input + len)) && \
// 		!ft_strchr(SPECIAL_TOKEN, *(input + len)))
// 	{
// 		if (*(input + len) == '\'')
// 			new->is_quoted = true;
// 		else if (*(input + len) == '\"')
// 			new->is_double_quoted = true;
// 		else if (*(input + len) == '$')
// 			new->is_dollar = true;
// 		len++;
// 	}
// 	return (len);
// }

// int	count_meta_len(char *line)
// {
// 	if (line[0] == '<' && line[1] == '<')
// 		return (2);
// 	if (line[0] == '>' && line[1] == '>')
// 		return (2);
// 	return (1);
// }

// int	count_envname_len(char *input, t_token *new, int start)
// {
// 	int	len;

// 	(void)start;
// 	len = 0;
// 	if (!(*input))
// 		return (0);
// 	while (ft_isspace(*(input + len) == false))
// 	{
// 		if (*(input + len) == '\'')
// 			new->is_quoted = true;
// 		else if (*(input + len) == '\"')
// 			new->is_double_quoted = true;
// 		len++;
// 	}
// 	return (len);
// }

// void	init_variables(int *i, int *single_flag, int *double_flag)
// {
// 	*i = 0;
// 	*single_flag = 0;
// 	*double_flag = 0;
// }
