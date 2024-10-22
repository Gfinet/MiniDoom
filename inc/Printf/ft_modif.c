/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_modif.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 14:37:13 by gfinet            #+#    #+#             */
/*   Updated: 2023/11/08 17:32:49 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*string_test(char *s)
{
	if (s)
		return (ft_strdup(s));
	else
		return (ft_strdup("(null)"));
}

int	check_conv(char c)
{
	if (c == '%')
		return (1);
	else if (c == 'c')
		return (2);
	else if (c == 's')
		return (3);
	else if (c == 'p')
		return (4);
	else if (c == 'd' || c == 'i')
		return (5);
	else if (c == 'u')
		return (6);
	else if (c == 'x')
		return (7);
	else if (c == 'X')
		return (8);
	return (0);
}

char	*pointer_modif(unsigned long arg)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_ultoa_base(arg, "0123456789abcdef");
	if (!tmp)
		return (0);
	tmp2 = ft_strjoin("0x", tmp);
	free(tmp);
	if (!tmp2)
		return (0);
	return (tmp2);
}
