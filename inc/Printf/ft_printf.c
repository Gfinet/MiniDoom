/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:30:31 by gfinet            #+#    #+#             */
/*   Updated: 2024/02/05 14:11:16 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	count_percent(const char *arg0)
{
	int		nb;
	int		i;

	nb = 0;
	i = 0;
	while (arg0[i])
	{
		if (arg0[i] == '%' && (arg0[i + 1]))
		{
			nb++;
			i++;
		}
		else if (!(arg0[i + 1]) && arg0[i] == '%')
			return (-1);
		i++;
	}
	return (nb);
}

static int	t_print_len(t_print **list)
{
	int		res;
	t_print	*tmp;

	res = 0;
	tmp = *list;
	while (tmp)
	{
		res += ft_strlen(tmp->content);
		if (tmp->type == 'c' && *(tmp->content) == '\0')
		{
			if (write(1, "\0", 1) == -1)
			{
				ft_plstclear(list, free);
				return (-1);
			}
			res++;
		}
		if (write(1, tmp->content, ft_strlen(tmp->content)) == -1)
		{
			ft_plstclear(list, free);
			return (-1);
		}
		tmp = tmp->next;
	}
	return (res + ft_plstclear(list, free));
}

int	ft_printf(const char *arg0, ...)
{
	va_list	ap;
	t_print	*p_list;
	int		nb_arg;

	p_list = 0;
	nb_arg = count_percent(arg0);
	if (nb_arg == -1)
		return (-1);
	if (!nb_arg)
	{
		if (write(1, (char *)arg0, ft_strlen(arg0)) == -1)
			return (-1);
		return (ft_strlen(arg0));
	}
	va_start(ap, arg0);
	if (!ft_make_list(arg0, &p_list, nb_arg, ap))
	{
		va_end(ap);
		return (-1);
	}
	va_end(ap);
	return (t_print_len(&p_list));
}
