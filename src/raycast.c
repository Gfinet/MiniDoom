/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 10:18:02 by Gfinet            #+#    #+#             */
/*   Updated: 2024/11/06 15:39:13 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"


double set_delta(double ray)
{
	if (ray)
		return (fabs(1 / ray));
	else
		return (1e30);
}

double	get_time(long start)
{
	struct timeval	tv;

	gettimeofday(&tv, 0);
	return ((double)tv.tv_sec * 1000 + tv.tv_usec / 1000 - start);
}

double	fix_texture_pos(t_rcdata dt, t_player pl)
{
	double	fix_x;
	double	garb;

	fix_x = 0;
	if (dt.side == 0 || dt.side == 2)
		fix_x = modf(pl.pos.y, &garb);
	else
		fix_x = modf(pl.pos.x, &garb);
	if ((pl.prev_pos.y != pl.pos.y) && (dt.side == 0 || dt.side == 2))
		fix_x = modf(pl.pos.y, &garb);
	else if ((pl.prev_pos.x != pl.pos.x) && (dt.side == 1 || dt.side == 3))
		fix_x = modf(pl.pos.x, &garb);
	else if ((pl.prev_pos.x != pl.pos.x) && (dt.side == 0 || dt.side == 2))
		fix_x = modf(pl.pos.y, &garb);
	else if ((pl.prev_pos.y != pl.pos.y) && (dt.side == 1 || dt.side == 3))
		fix_x = modf(pl.pos.x, &garb);
	return (fix_x);
}

static void	get_base_info_draw(t_drawdata *dr, t_rcdata dt, t_player player,
		t_cube *cube)
{
	double	fix_x;

	fix_x = fix_texture_pos(dt, player);
	(*dr).line_height = (int)(WIN_HEIGHT / dt.perp_wall_dist);
	(*dr).pitch = 100;

	(*dr).draw_start = -dr->line_height / 2 + WIN_HEIGHT / 2 + dr->pitch;
	if (dr->draw_start < 0)
		(*dr).draw_start = 0;

	(*dr).draw_end = dr->line_height / 2 + WIN_HEIGHT / 2 + dr->pitch;
	if (dr->draw_end >= WIN_HEIGHT)
		(*dr).draw_end = WIN_HEIGHT - 1;
	
	(*dr).tex_num = dt.side;
	if (dt.side == 0 || dt.side == 2)
		(*dr).wall_x = (int)player.pos.x + dt.perp_wall_dist * dt.rays.y;

	else if (dt.side == 1 || dt.side == 3)
		(*dr).wall_x = (int)player.pos.y + dt.perp_wall_dist * dt.rays.x;

	(*dr).wall_x += fix_x;
	(*dr).wall_x -= floor((dr->wall_x));
	(*dr).tex_x = (int)((dr->wall_x) * (double)(cube->texture[dt.side].width));
	if ((dt.side == 0 || dt.side == 2) && dt.rays.x > 0)
		(*dr).tex_x = cube->texture[dt.side].width - dr->tex_x - 1;
	if ((dt.side == 1 || dt.side == 3) && dt.rays.y < 0)
		(*dr).tex_x = cube->texture[dt.side].width - dr->tex_x - 1;
	(*dr).step_f = 1.0 * cube->texture[dt.side].height / dr->line_height;
}

static void	draw_xwall(t_data *screen, t_drawdata *dt, t_cube *c, int x)
{
	int				y;
	unsigned int	col;
	double			tex_pos;

	tex_pos = (dt->draw_start - dt->pitch - WIN_HEIGHT / 2 + dt->line_height
			/ 2) * dt->step_f;
	y = -1;
	col = c->lvl->floor[0] * 65536 + c->lvl->floor[1] * 256 + c->lvl->floor[2];
	if (dt->draw_start > WIN_HEIGHT)
		dt->draw_start = 0;
	while (++y < dt->draw_start)
		my_mlx_pixel_put(screen, x, y, col);
	y = dt->draw_start - 1;
	while (++y < dt->draw_end)
	{
		(*dt).tex_y = (int)tex_pos & (c->texture[dt->tex_num].height - 1);
		tex_pos += dt->step_f;
		col = *((unsigned int *)c->texture[dt->tex_num].addr +(int)
				((c->texture[dt->tex_num].height * dt->tex_y + dt->tex_x)));
		my_mlx_pixel_put(screen, x, y, col);
	}
	y -= 1;
	col = c->lvl->ceil[0] * 65536 + c->lvl->ceil[1] * 256 + c->lvl->ceil[2];
	while (++y < WIN_HEIGHT - 1)
		my_mlx_pixel_put(screen, x, y, col);
}

void raycasting(t_cube *cube)
{
	int			x;
	//int			en_id;
	t_rcdata	data;
	t_drawdata	draw;
	t_point *play_pos;

	play_pos = &cube->player->pos;
	data.pov.x = -(float)(0.66 * cube->player->dir.y);
	data.pov.y = (float)(0.66 * cube->player->dir.x);
	x = -1;
	set_draw_enemy(cube, 0);
	while (++x < WIN_WIDTH)
	{
		//data.rays = *play_pos;
		data.camerx = 2 * x / ((double)WIN_WIDTH) - 1; //x-coordinate in camera space
		data.rays.x = cube->player->dir.x + data.pov.x * data.camerx;
		data.rays.y = cube->player->dir.y + data.pov.y * data.camerx;
		//Digital Differential Analysis
		data.var.x = set_delta(data.rays.x);
		data.var.y = set_delta(data.rays.y);
		
		data.dest.x = (int)play_pos->x;
		data.dest.y = (int)play_pos->y;
		
		data.step.x = !(data.rays.x < 0) - (data.rays.x < 0);
		data.step.y = !(data.rays.y < 0) - (data.rays.y < 0);
		if (data.step.x == -1)
			data.side_dist.x = (play_pos->x - data.dest.x) * data.var.x;
		else
			data.side_dist.x = (1.0 + data.dest.x - play_pos->x) * data.var.x;
		if (data.step.y == -1)
			data.side_dist.y = (play_pos->y - data.dest.y) * data.var.y;
		else
			data.side_dist.y = (1.0 + data.dest.y - play_pos->y) * data.var.y;

		
		data.hit = '0';
		while (in_char_lst(data.hit, INVIS_WALL))
		{
			if (data.side_dist.x < data.side_dist.y)
    	    {
        		data.side_dist.x += data.var.x;
        		data.dest.x += data.step.x;
				data.side = 0;
				if (data.step.x > 0)
					data.side = 2;
      	  	}
       	 	else
        	{
        		data.side_dist.y += data.var.y;
       	   		data.dest.y += data.step.y;
				data.side = 1;
				if (data.step.y > 0)
					data.side = 3;
			}
			data.hit = cube->lvl->c_maps[(int)data.dest.y][(int)data.dest.x];
		}
		if (data.side % 2)
			data.perp_wall_dist = (data.side_dist.y - data.var.y);
		else
			data.perp_wall_dist = (data.side_dist.x - data.var.x);
			
		get_base_info_draw(&draw, data, *cube->player, cube);
		draw.tex_num = data.side;
		draw_xwall(&cube->screen, &draw, cube, x);
	}
	mlx_put_image_to_window(cube->mlx, cube->win, cube->screen.img, 0, 0);
}