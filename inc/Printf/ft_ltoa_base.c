/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ltoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 19:56:25 by gfinet            #+#    #+#             */
/*   Updated: 2023/11/08 16:38:35 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_compute_len(long int n, int base_size)
{
	int	len;

	len = 0;
	if (n == -9223372036854775807)
		return (18);
	if (n == 9223372036854775807)
		return (17);
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
		n /= base_size;
	}
	return (len);
}

// long min, len 20		long max, len 19
// -9223372036854775807	9223372036854775807

char	*ft_ltoa_base(long int n, char *base)
{
	int			f_len;
	int			len;
	long int	ncpy;
	char		*p;

	ncpy = n;
	len = ft_strlen(base);
	f_len = ft_compute_len(n, len);
	p = (char *)malloc((f_len + 1) * sizeof(char));
	if (!p)
		return (0);
	p[f_len--] = '\0';
	if (n < 0)
		ncpy = -ncpy;
	while (f_len >= 0)
	{
		p[f_len--] = base[ncpy % len];
		ncpy = ncpy / len;
	}
	if (n < 0)
		p[0] = '-';
	return (p);
}
// ulong max
// 18 446 744 073 709 551 615

static int	ft_compute_ulen(unsigned long n, int base_size)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		len++;
		n /= base_size;
	}
	return (len);
}

char	*ft_ultoa_base(unsigned long n, char *base)
{
	int					f_len;
	int					len;
	unsigned long		ncpy;
	char				*p;

	ncpy = n;
	len = ft_strlen(base);
	f_len = ft_compute_ulen(n, len);
	p = (char *)malloc((f_len + 1) * sizeof(char));
	if (!p)
		return (0);
	p[f_len--] = '\0';
	while (f_len >= 0)
	{
		p[f_len--] = base[ncpy % len];
		ncpy = ncpy / len;
	}
	return (p);
}
