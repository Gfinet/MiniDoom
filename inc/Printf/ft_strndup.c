/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 17:59:57 by gfinet            #+#    #+#             */
/*   Updated: 2023/11/04 20:21:56 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_strndup(const char *src, size_t n)
{
	char		*p;
	size_t		i;

	if (!src)
		return (0);
	i = 0;
	p = malloc((n + 1) * sizeof(char));
	if (!p)
		return (NULL);
	while (i < n)
	{
		p[i] = src[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}
