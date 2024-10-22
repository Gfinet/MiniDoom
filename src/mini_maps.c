/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_maps.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 19:05:21 by gfinet            #+#    #+#             */
/*   Updated: 2024/09/12 11:50:28 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

static int	is_border(int x, int y)
{
	if (x == 0 || y == 0 || x - 1 == 0 || y - 1 == 0)
		return (1);
	else if (x + 1 == WIN_HEIGHT / 5 || y + 1 == WIN_WIDTH / 5)
		return (1);
	else if (x + 2 == WIN_HEIGHT / 5 || y + 2 == WIN_WIDTH / 5)
		return (1);
	else
		return (0);
}

void	draw_mini_background(t_maps *lvl)
{
	int		i;
	int		j;
	t_data	*im;

	i = -1;
	im = &lvl->mini.maps;
	while (++i < WIN_HEIGHT / 5)
	{
		j = -1;
		while (++j < WIN_WIDTH / 5)
		{
			if (is_border(i, j))
				my_mlx_pixel_put(im, j, i, 0x55FFFFFF);
			else
				my_mlx_pixel_put(im, j, i, 0x550000FF);
		}
	}
}

int	*get_ind(int i[2], int w_h[2], t_maps *lvl)
{
	int	bf;
	int	af;
	int	coef[2];

	coef[0] = WIN_HEIGHT / 5 / lvl->mini.height;
	coef[1] = WIN_WIDTH / 5 / lvl->mini.witdh;
	af = 1;
	bf = 0;
	while (!(bf * coef[0] <= i[0] && i[0] < af * coef[0]))
	{
		bf++;
		af++;
	}
	w_h[0] = bf;
	af = 1;
	bf = 0;
	while (!(bf * coef[1] <= i[1] && i[1] < af * coef[1]))
	{
		bf++;
		af++;
	}
	w_h[1] = bf;
	return (w_h);
}

void	draw_maps(t_cube *cube)
{
	int		i[2];
	int		w_h[2];
	t_maps	*lvl;

	i[0] = -1;
	lvl = cube->lvl;
	draw_mini_background(lvl);
	while (++i[0] < WIN_HEIGHT / 5)
	{
		i[1] = -1;
		while (++i[1] < WIN_WIDTH / 5)
		{
			get_ind(i, w_h, lvl);
			if (w_h[0] < lvl->mini.height - 2)
			{
				if (w_h[1] < lvl->max_len)
					draw_mini_pixel(lvl, w_h, i);
			}
		}
	}
	draw_player(cube);
	mlx_put_image_to_window(cube->mlx, cube->win,
		cube->lvl->mini.maps.img, 4 * WIN_WIDTH / 5, 0);
}

int	make_mini(t_cube *cube, t_maps *lvl)
{
	new_img(cube, &(lvl->mini.maps), WIN_WIDTH / 5, WIN_HEIGHT / 5);
	lvl->mini.witdh = lvl->max_len + 1;
	lvl->mini.height = lvl->m_height + 2;
	draw_maps(cube);
	get_player_pos(cube);
	return (1);
}
