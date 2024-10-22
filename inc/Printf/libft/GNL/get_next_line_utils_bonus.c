/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvodak <lvodak@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 18:03:06 by gfinet            #+#    #+#             */
/*   Updated: 2024/07/13 17:47:53 by lvodak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*free_all_gnl(char **tmp, char **tmp2)
{
	if (tmp != NULL)
	{
		free(*tmp);
		*tmp = NULL;
	}
	if (tmp2 != NULL)
	{
		free(*tmp2);
		*tmp2 = NULL;
	}
	return (NULL);
}

char	*ft_stradd(char *s1, char const *s2)
{
	size_t		j;
	size_t		i;
	char		*p;

	if (!s1 && !s2)
		return (0);
	p = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	i = 0;
	j = 0;
	if (p == NULL)
		return (free_all_gnl(&s1, 0));
	while (s1 && i < ft_strlen(s1))
	{
		p[i] = s1[i];
		i++;
	}
	free(s1);
	while (s2 && j < ft_strlen(s2))
	{
		p[i + j] = s2[j];
		j++;
	}
	p[i + j] = '\0';
	return (p);
}

char	*ft_strchr(const char *str, int c)
{
	char	*p;
	size_t	len;

	if (str == NULL)
		return (0);
	len = ft_strlen(str);
	p = (char *)str;
	while (p - 1 != &str[len])
	{
		if (*p == (char)c)
			return (p);
		p++;
	}
	if (c == '\0')
		return (p);
	return (0);
}
