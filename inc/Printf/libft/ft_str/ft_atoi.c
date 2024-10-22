/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 18:07:49 by gfinet            #+#    #+#             */
/*   Updated: 2024/01/31 19:15:04 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	check_isspace(char c)
{
	if (c == ' ')
		return (1);
	return (c == '\f' || c == '\n' || c == '\t' || c == '\r' || c == '\v');
}

int	ft_atoi(const char *str)
{
	int	i;
	int	res;
	int	minus;

	i = 0;
	res = 0;
	minus = 0;
	while (check_isspace(str[i]))
		i++;
	minus = (str[i] == '-');
	i += (str[i] == '-' || str[i] == '+');
	while (ft_isdigit(str[i]))
	{
		res *= 10;
		res += str[i] - '0';
		i++;
	}
	if (minus)
		res *= -1;
	return (res);
}
