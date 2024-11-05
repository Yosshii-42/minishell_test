/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_token_lst.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-05 06:28:22 by yotsurud          #+#    #+#             */
/*   Updated: 2024/11/05 16:44:39 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token	*lstlast(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

static void	lstadd_back(t_token **start, t_token *new)
{
	t_token	*ptr;

	ptr = NULL;
	if (!*start)
	{
		*start = new;
		new->pre = NULL;
		return ;
	}
	ptr = lstlast(*start);
	ptr->next = new;
	new->pre = ptr;
}

static void	lstnew(t_token **start, char *token)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return ;
	new->word = ft_strdup(token);
	if (!(new->word))
		return ;
	new->kind = -1;
	new->status = -1;
	new->next = NULL;
	lstadd_back(start, new);
}

t_token	*make_token_lst(char *line, int status_num)
{
	char	**split;
	int		i;
	t_token	*token;

	if (!line)
		return (NULL);
	split = ft_split(line, ' ');
	if (!split)
		return (NULL);
	token = NULL;
	i = -1;
	while (split[++i])
		lstnew(&token, split[i]);//nullが帰った時の処理
	add_token_kind(token, status_num);
	free_split(split);
	return (token);
}
