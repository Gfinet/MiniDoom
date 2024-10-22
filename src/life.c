/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 19:01:31 by Gfinet            #+#    #+#             */
/*   Updated: 2024/09/12 11:50:28 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

int	set_life(t_cube *cube)
{
	cube->player->hp = 50;
	if (!new_img(cube, &cube->player->life, WIN_WIDTH / 5, 20))
		return (0);
	return (1);
}

void	draw_life(t_cube *cube)
{
	int	y;
	int	x;
	int	hp_r;

	y = -1;
	hp_r = WIN_WIDTH / 5 / (100 / cube->player->hp);
	while (++y < 20)
	{
		x = -1;
		while (++x < WIN_WIDTH / 5)
		{
			if (y == 0 || y == 19 || x == 0 || x + 1 == WIN_WIDTH / 5)
				my_mlx_pixel_put(&cube->player->life, x, y, 0x00FFFFFF);
			else if (x < hp_r)
				my_mlx_pixel_put(&cube->player->life, x, y, 0x0000FF00);
			else
				my_mlx_pixel_put(&cube->player->life, x, y, 0x00FF0000);
		}
	}
	mlx_put_image_to_window(cube->mlx, cube->win, cube->player->life.img,
		4 * WIN_WIDTH / 5, WIN_HEIGHT / 5);
	return ;
}
