/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:05:01 by gfinet            #+#    #+#             */
/*   Updated: 2024/01/31 19:15:28 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strnstr(const char *b, const char *l, size_t len)
{
	size_t		i;
	size_t		j;

	if (!l[0])
		return ((char *)&b[0]);
	i = 0;
	while (i < len && b[i])
	{
		j = 0;
		if (b[i] == l[j])
		{
			while (b[i + j] == l[j] && i + j < len && b[i + j] && l[j])
				j++;
			if (l[j] == '\0')
				return ((char *)&b[i]);
		}
		i++;
	}
	return (0);
}
