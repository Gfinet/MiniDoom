/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 20:53:24 by gfinet            #+#    #+#             */
/*   Updated: 2024/01/31 19:13:49 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*prev;
	t_list	*next_node;

	prev = *lst;
	if (prev && del)
	{
		while (prev)
		{
			next_node = prev->next;
			ft_lstdelone(prev, del);
			prev = next_node;
		}
	}
	*lst = 0;
}
