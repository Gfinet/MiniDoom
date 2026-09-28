/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ui.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 13:48:26 by Gfinet            #+#    #+#             */
/*   Updated: 2026/09/26 21:24:55 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void draw_ui(t_cube *cube)
{
	t_player	*play;

	play = cube->player;
	if (cube->lvl->weap)
		draw_weapons(cube);
	// draw_mini_background(cube->lvl);
	draw_maps(cube);
	draw_life(cube);
	mlx_image_to_image(&cube->screen, &cube->ui.box, 0, WIN_HEIGHT * 4 / 5);//WIN_HEIGHT * 9 / 10
	mlx_string_put(cube->mlx, cube->win,
		WIN_WIDTH / 2, WIN_HEIGHT / 2, 0x000000FF, play->hp_val);
	mlx_string_put(cube->mlx, cube->win,
		WIN_WIDTH / 2, WIN_HEIGHT / 2, 0x000000FF, play->ar_val);
	free(play->ar_val);
	free(play->hp_val);
}

void draw_box_frame(t_data *box)
{
	printf("%d %d\n", box->height, box->width);
	printf("%d %d\n", WIN_HEIGHT, WIN_WIDTH);
	for (int i = 0 ; i < box->width; i++)
		for (int j = 0; j < box->height; j++)
			if ((i < 2 || i >= box->width - 2) ||
				j < 2 || j >= box->height - 2)
				my_mlx_pixel_put(box, i, j, 0x00010101);
			else if ((i < 5 || i >= box->width - 5) ||
				j < 5 || j >= box->height - 5)
				my_mlx_pixel_put(box, i, j, 0x00AAAAAA);
	// for (int i = 0 ; i < box->width - 1; i++)
	// 	if (i == 0 || i == box->width - 2)
	// 		my_mlx_pixel_put(box, 0, i, 0xFFFFFFFF);

}

int make_ui(t_cube *cube)
{
	t_data *box;

	box = &cube->ui.box;
	if (!new_img(cube, box, WIN_WIDTH, WIN_HEIGHT / 5))
		return (0);

	draw_box_frame(box);
	return 1;
}