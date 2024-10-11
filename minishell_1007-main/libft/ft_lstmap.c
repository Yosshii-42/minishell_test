/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 10:01:02 by yotsurud          #+#    #+#             */
/*   Updated: 2024/04/22 17:36:03 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*begin;
	t_list	*new_lst;
	t_list	*new_node;

	if (!lst || !f || !del)
		return (NULL);
	begin = ft_lstnew(f(lst->content));
	if (!begin)
		return (NULL);
	new_lst = begin;
	while (lst->next)
	{
		lst = lst->next;
		new_node = ft_lstnew(f(lst->content));
		if (!new_node)
		{
			ft_lstclear(&begin);
			return (NULL);
		}
		new_lst->next = new_node;
		new_lst = new_lst->next;
	}
	return (begin);
}
