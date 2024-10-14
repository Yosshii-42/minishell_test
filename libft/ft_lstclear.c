/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotsurud <yotsurud@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 19:36:52 by yotsurud          #+#    #+#             */
/*   Updated: 2024/04/22 17:09:39 by yotsurud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst)
{
	t_list	*address;

	if (!lst || !(*lst))
		return ;
	while (*lst)
	{
		address = (*lst)->next;
		ft_lstdelone(*lst);
		*lst = address;
	}
	lst = NULL;
}
