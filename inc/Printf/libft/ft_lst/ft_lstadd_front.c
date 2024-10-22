/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 20:53:31 by gfinet            #+#    #+#             */
/*   Updated: 2024/01/31 19:13:38 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	t_list	*p;

	if (!new)
		return ;
	if (!*lst)
		*lst = new;
	else
	{
		p = *lst;
		*lst = new;
		new->next = p;
	}
}
