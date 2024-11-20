/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 21:49:16 by Gfinet            #+#    #+#             */
/*   Updated: 2024/11/19 21:50:46 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

double dist_ab(t_point a, t_point b)
{
	double	carr_x, carr_y;
	double	res;

	carr_x = (a.x - b.x);
	carr_x *= carr_x;
	carr_y = (a.y - b.y);
	carr_y *= carr_y;
	res = sqrt(carr_x + carr_y);
	return (res);
}

double ft_modf(double numb, double *integ)
{
	double	fract;

	*integ = (double)(int)numb;
	fract = numb - *integ;
	return (fract);
}
