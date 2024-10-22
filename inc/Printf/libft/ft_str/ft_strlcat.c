/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:14:25 by gfinet            #+#    #+#             */
/*   Updated: 2024/01/31 19:15:18 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t dstlen)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(dest);
	while (len + i + 1 < dstlen && src[i])
	{
		dest[len + i] = src[i];
		i++;
	}
	if (dstlen || len <= dstlen)
		dest[len + i] = '\0';
	if (dstlen <= len)
		return (dstlen + ft_strlen(src));
	return (len + ft_strlen(src));
}
