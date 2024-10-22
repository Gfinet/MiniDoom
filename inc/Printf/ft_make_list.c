/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_make_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 13:03:03 by gfinet            #+#    #+#             */
/*   Updated: 2023/11/08 17:32:34 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	**modif_current(char **current, const char *arg0, va_list ap)
{
	char	tmp[2];

	tmp[1] = '\0';
	if (check_conv(*(arg0)) == 1)
		*current = ft_strdup("%\0");
	else if (check_conv(*(arg0)) == 2)
	{
		tmp[0] = va_arg(ap, int);
		*current = ft_strndup(tmp, 2);
	}
	else if (check_conv(*(arg0)) == 3)
		*current = string_test(va_arg(ap, char *));
	else if (check_conv(*(arg0)) == 4)
		*current = pointer_modif(va_arg(ap, unsigned long));
	else if (check_conv(*(arg0)) == 5)
		*current = ft_ltoa_base(va_arg(ap, int), "0123456789");
	else if (check_conv(*(arg0)) == 6)
		*current = ft_ltoa_base(va_arg(ap, unsigned int), "0123456789");
	else if (check_conv(*(arg0)) == 7)
		*current = ft_ultoa_base(va_arg(ap, unsigned int), "0123456789abcdef");
	else if (check_conv(*(arg0)) == 8)
		*current = ft_ultoa_base(va_arg(ap, unsigned int), "0123456789ABCDEF");
	return (current);
}

static int	ft_add_p_arg(char *arg0, t_print **p_list, int n, char c)
{
	t_print	*p;
	char	*tmp;

	tmp = ft_strndup(arg0, n);
	if (!tmp)
		return (ft_plstclear(p_list, free));
	p = ft_plstnew(tmp);
	if (!p)
	{
		free(tmp);
		return (ft_plstclear(p_list, free));
	}
	p->type = c;
	ft_plstadd_back(p_list, p);
	return (1);
}

static int	ft_get_arg(const char *arg0, t_print **p_list, va_list ap)
{
	char	*current;

	if (!check_conv(*arg0))
		return (ft_plstclear(p_list, free));
	modif_current(&current, arg0, ap);
	if (!current)
		return (ft_plstclear(p_list, free));
	if (!ft_add_p_arg(current, p_list, ft_strlen(current), *arg0))
	{
		free(current);
		return (ft_plstclear(p_list, free));
	}
	free(current);
	return (1);
}

int	ft_make_list(const char *arg0, t_print **p_list, int nb_arg, va_list ap)
{
	int	get_arg;
	int	j;

	get_arg = 0;
	while (*arg0)
	{
		j = 0;
		while (arg0[j] != '%' && arg0[j])
			j++;
		if (j)
		{
			if (!ft_add_p_arg((char *)arg0, p_list, j, 0))
				return (0);
		}
		if ((arg0[j] != '\0') && get_arg < nb_arg)
		{
			if (!ft_get_arg(&arg0[j + 1], p_list, ap))
				return (0);
			get_arg++;
		}
		arg0 += j + 2 * (arg0[j] != '\0');
	}
	return (1);
}
