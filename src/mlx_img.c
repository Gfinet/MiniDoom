/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_img.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 22:03:11 by gfinet            #+#    #+#             */
/*   Updated: 2024/12/26 05:31:06 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void	draw_mini_pixel(t_maps *lvl, int w_h[2], int i[2])
{
	int	x;
	int	y;

	x = i[1] + WIN_WIDTH / 5 / (lvl->mini.witdh);
	y = i[0] + WIN_HEIGHT / 5 / (lvl->mini.height);
	if (lvl->c_maps[w_h[0]][w_h[1]] == '1'
		|| lvl->c_maps[w_h[0]][w_h[1]] == '2')
		my_mlx_pixel_put(&lvl->mini.maps, x, y, WHITE + 0x55000000);
	else if (lvl->c_maps[w_h[0]][w_h[1]] == '0')
		my_mlx_pixel_put(&lvl->mini.maps, x, y, GREEN);
	else if (lvl->c_maps[w_h[0]][w_h[1]] == 'D')
		my_mlx_pixel_put(&lvl->mini.maps, x, y, 0x55880088);
	else if (lvl->c_maps[w_h[0]][w_h[1]] == '.'
		|| lvl->c_maps[w_h[0]][w_h[1]] == 0)
		my_mlx_pixel_put(&lvl->mini.maps, x, y, BLUE + 0x55000000);
	else
		my_mlx_pixel_put(&lvl->mini.maps, x, y, GREEN);
}

void	draw_doom(t_cube *cube)
{
	mlx_clear_window(cube->mlx, cube->win);
	raycasting(cube);
	draw_player(cube);
	raycast_enemy(cube);
	draw_enemies(cube);
	if (cube->lvl->weap)
		draw_weapons(cube);
	draw_mini_background(cube->lvl);
	draw_maps(cube);
	draw_life(cube);
}

int	xpm_to_img(t_cube *cube, t_data *new_img, char *name)
{
	new_img->img = mlx_xpm_file_to_image(cube->mlx, name,
			&new_img->width, &new_img->height);
	if (!new_img->img)
		return (0);
	new_img->addr = mlx_get_data_addr(new_img->img, &new_img->bits_per_pixel,
			&new_img->line_length, &new_img->endian);
	return (1);
}

int	new_img(t_cube *cube, t_data *new_img, int width, int height)
{
	new_img->img = mlx_new_image(cube->mlx, width, height);
	if (!new_img->img)
		return (0);
	new_img->addr = mlx_get_data_addr(new_img->img, &new_img->bits_per_pixel,
			&new_img->line_length, &new_img->endian);
	return (1);
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;

}

unsigned int get_color_from_xpm(t_data *text, int x, int y)
{
	int				pixel;
	unsigned int	color;

	pixel = (int)(text->line_length * y / 4 + x * (text->bits_per_pixel / 32));
	color = *((unsigned int *)text->addr + pixel);
	return (color);
}