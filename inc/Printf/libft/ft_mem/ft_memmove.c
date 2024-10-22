/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 22:07:06 by gfinet            #+#    #+#             */
/*   Updated: 2024/01/31 19:14:45 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memmove(void *dest, const void *src, size_t len)
{
	unsigned char	*s1;
	unsigned char	*s2;
	size_t			i;

	i = 0;
	s1 = (unsigned char *)src;
	s2 = (unsigned char *)dest;
	if (s2 > s1)
	{
		while (++i <= len)
			s2[len - i] = s1[len - i];
	}
	else
	{
		while (len-- > 0)
			*s2++ = *s1++;
	}
	return (dest);
}
