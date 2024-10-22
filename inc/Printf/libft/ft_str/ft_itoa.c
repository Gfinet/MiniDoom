/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 19:56:25 by gfinet            #+#    #+#             */
/*   Updated: 2024/01/31 19:15:06 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_compute_len(int n)
{
	int	len;

	len = 0;
	if (n == -2147483648)
		return (11);
	if (n == 0)
		return (1);
	if (n < 0)
	{
		len ++;
		n = -n;
	}
	while (n)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int			f_len;
	long int	ncpy;
	char		*p;

	f_len = ft_compute_len(n);
	ncpy = n;
	p = (char *)malloc((f_len + 1) * sizeof(char));
	if (!p)
		return (0);
	p[f_len--] = '\0';
	if (n < 0)
		ncpy = -ncpy;
	while (f_len >= 0)
	{
		p[f_len--] = (ncpy % 10) + '0';
		ncpy /= 10;
	}
	if (n < 0)
		p[0] = '-';
	return (p);
}
