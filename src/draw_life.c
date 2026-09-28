/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_life.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 19:01:31 by Gfinet            #+#    #+#             */
/*   Updated: 2026/09/26 21:25:45 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

int	set_life(t_cube *cube)
{
	cube->player->hp = 100;
	if (!new_img(cube, &cube->player->life, WIN_WIDTH / 5, 20))
		return (0);
	cube->player->armor = 50;
	if (!new_img(cube, &cube->player->shield, WIN_WIDTH / 5, 20))
		return (0);
	return (1);
}

void	draw_life(t_cube *cube)
{
	int			y;
	int			x;
	int			hp_r, ar_r;
	char 		*tmp;
	t_data		*life, *box, *shield;

	y = -1;
	life = &cube->player->life;
	shield = &cube->player->shield;
	box = &cube->ui.box;
	
	tmp = ft_strjoin(ft_itoa(cube->player->hp), " / ");
	cube->player->hp_val = ft_strjoin(tmp, ft_itoa(MAX_LIFE));
	free(tmp);
	tmp = ft_strjoin(ft_itoa(cube->player->armor), " / ");
	cube->player->ar_val = ft_strjoin(tmp, ft_itoa(MAX_LIFE));
	free(tmp);

	hp_r = WIN_WIDTH / 5 / (100 / cube->player->hp);
	ar_r = WIN_WIDTH / 5 / (100 / cube->player->armor);
	while (++y < 20)
	{
		x = -1;
		while (++x < WIN_WIDTH / 5)
		{
			if (y == 0 || y == 19 || x == 0 || x + 1 == WIN_WIDTH / 5)
			{
				my_mlx_pixel_put(life, x, y, 0x00FFFFFF);
				my_mlx_pixel_put(shield, x, y, 0x00FFFFFF);
			}
			if (x < hp_r)
				my_mlx_pixel_put(life, x, y, 0x0000FF00);
			else
				my_mlx_pixel_put(life, x, y, 0x00FF0000);
			if (x < ar_r)
				my_mlx_pixel_put(shield, x, y, 0x000000FF);
			else
				my_mlx_pixel_put(shield, x, y, 0x00FFFFFF);
		}
	}
	mlx_image_to_image(&cube->ui.box, life, (box->width - life->width ) / 2, 5);
	mlx_image_to_image(&cube->ui.box, shield, (box->width - shield->width ) / 2, 5 + life->height);
	// printf("%s %s\n", hp_val, ar_val);
	// mlx_put_image_to_window(cube->mlx, cube->win, cube->player->life.img,
	// 	4 * WIN_WIDTH / 5, WIN_HEIGHT / 5);
	return ;
}
