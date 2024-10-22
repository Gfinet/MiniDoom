/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 20:53:07 by gfinet            #+#    #+#             */
/*   Updated: 2024/01/31 19:14:23 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdio.h>

static t_list	*free_all(t_list **start, void **tmp, void (*del)(void *))
{
	del(*tmp);
	ft_lstclear(start, del);
	return (0);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_node;
	t_list	*prev_node;
	t_list	*start;
	void	*tmp;

	if (!lst)
		return (NULL);
	tmp = f(lst->content);
	start = ft_lstnew(tmp);
	if (!start)
		return (free_all(&start, &tmp, del));
	prev_node = start;
	while (lst->next)
	{
		lst = lst->next;
		tmp = f(lst->content);
		new_node = ft_lstnew(tmp);
		if (!new_node)
			return (free_all(&start, &tmp, del));
		prev_node->next = new_node;
		prev_node = prev_node->next;
	}
	return (start);
}

// int main()
// {
// 	t_list *p, *p1, *p2, *p3, *p4, *p5, *q;
// 	void 	*s, *x;

// 	s = (void *)ft_strlen;
// 	x = (void *)free;
// 	p = NULL;

// 	p1 = ft_lstnew(ft_strdup("one"));
// 	p2 = ft_lstnew(ft_strdup("two"));
// 	p3 = ft_lstnew(ft_strdup("three"));
// 	p4 = ft_lstnew(ft_strdup("four"));
// 	p5 = ft_lstnew(ft_strdup("five"));
// 	ft_lstadd_back(&p, p1);
// 	ft_lstadd_back(&p, p2);
// 	ft_lstadd_back(&p, p3);
// 	ft_lstadd_back(&p, p4);
// 	ft_lstadd_back(&p, p5);

// 	//ft_lstmap(p, ft_strlen, free);
// 	q = ft_lstmap(p, s, x);
// 	free(p);
// 	return 0;
// }