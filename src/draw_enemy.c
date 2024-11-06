/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_enemy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:29:12 by Gfinet            #+#    #+#             */
/*   Updated: 2024/11/06 16:07:32 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void raycast_enemy(t_cube *cube)
{
	int			x;
	int			en_id;
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
			en_id = enemy_in_sight(cube, &data);
			if (en_id)
			{
				data.hit = '!';
				break;
			}
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
		else if (data.side == 3 || data.side == 1)
			data.perp_wall_dist = (data.side_dist.x - data.var.x);
		if (en_id)
			draw_enemy(cube, &draw, x, en_id);
	}
	//mlx_put_image_to_window(cube->mlx, cube->win, cube->screen.img, 0, 0);
}

int enemy_in_sight(t_cube *cube, t_rcdata *data)
{
	int		i;
	t_enemy	*adv;
	t_point	posi;
	//t_point hit;

	adv = cube->lvl->enemy;
	i = 0;
	while (i < cube->lvl->nb_enemy)
	{
		posi = adv[i].pos;
		//hit = adv[i].hitbox;
		if (!adv[i].draw && posi.x  < (int)data->dest.x && posi.x > (int)data->dest.x - 1 \
		&& posi.y > (int)data->dest.y && posi.y < (int)data->dest.x + 1)
			return (adv[i].draw = 1, adv[i].id);
		i++;
	}
	return (0);
}

void draw_enemy(t_cube *cube, t_drawdata *draw, int x, int id)
{
	int			i;
	int			wid, hei;
	//static int	nb_draw = 0;
	t_enemy 	*adv;

	(void)draw;
	i = -1;
	while (++i < cube->lvl->nb_enemy)
		if (cube->lvl->enemy[i].id == id)
			adv = &cube->lvl->enemy[i];
	if (adv->spr_fr)
		mlx_destroy_image(cube->mlx, adv->spr_fr->img);
	new_img(cube, &adv->spr_fr, wid, hei);
	mlx_put_image_to_window(cube->mlx, cube->win, adv->spr_fr->img, x, WIN_HEIGHT / 2);
}
