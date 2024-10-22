/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 17:25:37 by gfinet            #+#    #+#             */
/*   Updated: 2024/01/31 19:15:32 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static size_t	look_start_end(const char *s1, const char *set, int st_en)
{
	size_t	out;
	int		stop;

	stop = 0;
	out = 0;
	if (!s1 || !set)
		return (0);
	if (st_en)
		out = ft_strlen(s1) - 1;
	if (!ft_strlen(s1))
		return (0);
	while (s1[out] && ft_strchr(set, s1[out]) && !stop)
	{
		if (!st_en)
			out++;
		else if (st_en && &s1[out] != s1)
			out--;
		else if (st_en && &s1[out] == s1)
			stop = 1;
	}
	return (out);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	size_t		end;
	size_t		start;
	size_t		i;
	char		*p;

	if (!s1 || !set)
		return (0);
	start = look_start_end(s1, set, 0);
	end = look_start_end(s1, set, 1);
	if (end < start || (end == 0 && start == 0))
		return (ft_strdup(""));
	p = (char *)malloc((end - start + 2) * sizeof(char));
	if (!p)
		return (0);
	i = 0;
	while (i <= end - start)
	{
		p[i] = s1[start + i];
		i++;
	}
	p[i] = '\0';
	return (p);
}
