/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_plst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 14:39:13 by gfinet            #+#    #+#             */
/*   Updated: 2023/11/08 17:13:23 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_plstadd_back(t_print **lst, t_print *new)
{
	t_print	*p;

	if (!*lst)
		*lst = new;
	else
	{
		p = *lst;
		while (p->next)
			p = p->next;
		p->next = new;
	}
}

int	ft_plstclear(t_print **lst, void (*del)(void *))
{
	t_print	*prev;
	t_print	*next_node;

	prev = *lst;
	while (prev)
	{
		next_node = prev->next;
		ft_plstdelone(prev, del);
		prev = next_node;
	}
	*lst = 0;
	return (0);
}

void	ft_plstdelone(t_print *lst, void (*del)(void *))
{
	if (lst && del)
	{
		if (lst->content)
			del(lst->content);
		free(lst);
	}
}

t_print	*ft_plstnew(char *cont)
{
	t_print	*new;

	new = (t_print *)malloc(sizeof(t_print));
	if (!new)
		return (0);
	new->content = cont;
	new->next = 0;
	return (new);
}
