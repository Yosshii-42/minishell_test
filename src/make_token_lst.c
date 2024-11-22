/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_token_lst.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/11/20 17:08:36 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//static t_token	*lstlast(t_token *lst)
//{
//	if (!lst)
//		return (NULL);
//	while (lst->next)
//		lst = lst->next;
//	return (lst);
//}

//static void	lstadd_back(t_token **start, t_token *new)
//{
//	t_token	*ptr;

//	ptr = NULL;
//	if (!*start)
//	{
//		*start = new;
//		new->pre = NULL;
//		return ;
//	}
//	ptr = lstlast(*start);
//	ptr->next = new;
//	new->pre = ptr;
//}

//static void	lstnew(t_token **start, char *token)
//{
//	t_token	*new;

//	new = (t_token *)malloc(sizeof(t_token));
//	if (!new)
//		return ;
//	new->word = ft_strdup(token);
//	if (!(new->word))
//		return ;
//	new->kind = -1;
//	new->status = -1;
//	new->next = NULL;
//	lstadd_back(start, new);
//}

t_token	*make_token_lst(char *line, t_env *env, int status_num)
{
	t_token	*token;
	int		error_status;

	error_status = 0;
	(void)*env;
	if (!(*line))
		return (NULL);
	token = NULL;
	token = lexer(env, line, &error_status);
	if (error_status || !token)
		return (NULL);
	add_token_kind(token, status_num);
	return (token);
}
