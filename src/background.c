/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 11:32:16 by Gfinet            #+#    #+#             */
/*   Updated: 2024/09/12 12:35:32 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void	draw_background(t_cube *cube)
{
	int x;
	int y;
	int ceil;
	int floor;

	ceil = cube->lvl->ceil[0] * 65536 + cube->lvl->ceil[1] * 256 + cube->lvl->ceil[2];
	floor = cube->lvl->floor[0] * 65536 + cube->lvl->floor[1] * 256 + cube->lvl->floor[2];

	y = -1;
	while (++y < WIN_HEIGHT)
	{
		x = -1;
		while (++x < WIN_WIDTH)
		{
			if (y < WIN_HEIGHT / 2)
				my_mlx_pixel_put(&cube->screen, x, y, ceil);
			else
				my_mlx_pixel_put(&cube->screen, x, y, floor);
		}
	}
	mlx_put_image_to_window(cube->mlx, cube->win, cube->screen.img, 0, 0);
}