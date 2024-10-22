/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:48:45 by gfinet            #+#    #+#             */
/*   Updated: 2024/01/31 19:15:30 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strrchr(const char *str, int c)
{
	char	*p;

	p = (char *)str;
	while (*p)
		p++;
	while (p + 1 != str)
	{
		if (*p == (char)c)
			return (p);
		p--;
	}
	if (!c)
		return (p);
	return (0);
}
