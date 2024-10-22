/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 17:48:44 by gfinet            #+#    #+#             */
/*   Updated: 2024/01/31 19:15:08 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	compute_nb(char const *s, char c)
{
	int		nb;
	int		i;
	int		flag;

	nb = 0;
	i = 0;
	flag = 0;
	while (s[i])
	{
		if (s[i] == c)
			flag = 0;
		else if (!flag)
		{
			flag = 1;
			nb++;
		}
		i++;
	}
	return (nb);
}

static char	*ft_strndup(const char *src, size_t n)
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

static char	**free_all(char **l, int n)
{
	int	i;

	i = 0;
	while (i <= n)
		free(l[i++]);
	free(l);
	return (0);
}

static char	**make_list(char const *s, char c, int nb)
{
	char	**l;
	int		i;
	int		j;
	int		k;

	i = 0;
	k = 0;
	l = (char **)malloc((nb) * sizeof(char *));
	if (!l)
		return (0);
	while (k < nb - 1)
	{
		while (s[i] == c && s[i])
			i++;
		j = 1;
		while (s[i + j] != c && s[i + j])
			j++;
		l[k] = ft_strndup(&s[i], j);
		if (!l[k])
			return (free_all(l, k));
		i += j;
		k++;
	}
	l[k] = 0;
	return (l);
}

char	**ft_split(char const *s, char c)
{
	int		nb;

	if (!s)
		return (0);
	nb = compute_nb(s, c) + 1;
	return (make_list(s, c, nb));
}
