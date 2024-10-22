/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_img.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 22:03:11 by gfinet            #+#    #+#             */
/*   Updated: 2024/10/07 17:17:39 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void	get_player_pos(t_cube *cube)
{
	int		i[2];
	char	c;
	char	**m;

	m = cube->lvl->c_maps;
	i[0] = -1;
	c = 0;
	while (++i[0] < cube->lvl->m_height && !c)
	{
		i[1] = -1;
		while (i[1]++ < cube->lvl->max_len)
		{
			if (m[i[0]][i[1]] == 'N' || m[i[0]][i[1]] == 'S'
			|| m[i[0]][i[1]] == 'E' || m[i[0]][i[1]] == 'W')
			{
				c = m[i[0]][i[1]];
				cube->player->pos = (t_point){i[1] + 0.1, i[0] + 0.1};
				break ;
			}
		}
	}
	cube->player->dir = (t_point){(c == 'E') - (c == 'W'),
		(c == 'S') - (c == 'N')};
	cube->player->prev_pos = (t_point){cube->player->pos.x,
		cube->player->pos.y};
}

void	fill_map_char(t_maps *lvl, char c)
{
	int		i;
	int		j;
	char	ch;
	size_t	len;

	i = -1;
	while (++i < lvl->m_height)
	{
		j = -1;
		len = ft_strlen(lvl->c_maps[i]);
		while (++j < lvl->max_len)
		{
			ch = lvl->c_maps[i][j];
			if (!in_char_lst(ch, LST_CHAR)
				|| (j > (int)len && j < lvl->max_len - 1))
				lvl->c_maps[i][j] = c;
			if (j == lvl->max_len - 1)
				lvl->c_maps[i][j] = 0;
		}
	}
}

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

void	draw_player(t_cube *cube)
{
	int	x;
	int	y;
	int	pos[2];
	int	sum[2];

	pos[0] = cube->player->pos.x + 1;
	pos[1] = cube->player->pos.y + 1;
	sum[0] = WIN_WIDTH / 5 / (cube->lvl->mini.witdh);
	sum[1] = WIN_HEIGHT / 5 / (cube->lvl->mini.height);
	y = -1;
	while (++y < WIN_HEIGHT / 5)
	{
		x = -1;
		while (++x < WIN_WIDTH / 5)
			if (x / sum[0] >= pos[0] && x / sum[0] < pos[0] + 1
				&& y / sum[1] >= pos[1] && y / sum[1] < pos[1] + 1)
				my_mlx_pixel_put(&cube->lvl->mini.maps, x, y, RED);
	}
}

void	draw_doom(t_cube *cube)
{
	mlx_clear_window(cube->mlx, cube->win);
	draw_background(cube);
	raycasting(cube);
	draw_mini_background(cube->lvl);
	draw_maps(cube);
	//draw_player(cube);
	//if (cube->lvl->weap)
	//	draw_weapons(cube, 0);
	draw_life(cube);
}
