/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_enemy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:29:12 by Gfinet            #+#    #+#             */
/*   Updated: 2024/11/08 02:04:07 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void raycast_enemy(t_cube *cube)
{
	int			x;
	int			en_id;
	int			center_flag = 0;
	t_rcdata	data;
	t_point *play_pos;
	
	play_pos = &cube->player->pos;
	data.pov.x = -(float)(0.66 * cube->player->dir.y);
	data.pov.y = (float)(0.66 * cube->player->dir.x);
	x = -1;
	data.center = (t_point){0};
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
		if (x == WIN_WIDTH / 2 && !center_flag)
			data.center = data.dest;
		if (en_id)
			draw_enemy(cube, data.center, x, en_id);
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
		if (!adv[i].draw \
		&& (int)posi.x  == (int)data->dest.x \
		&& (int)posi.y == (int)data->dest.y ) // && posi.y < (int)data->dest.x + 1) // && posi.x < (int)data->dest.x + 1 
			return (adv[i].draw = 1, adv[i].id);
		i++;
	}
	return (0);
}

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
void blabla(t_cube *c, t_drawdata *dt, int x, t_data *screen)
{
	unsigned int col;
	int y;
	double tex_pos = 0.0;
	
	y = dt->draw_start - 1;
	while (++y < dt->draw_end)
	{
		(*dt).tex_y = (int)tex_pos & (c->texture[dt->tex_num].height - 1);
		tex_pos += dt->step_f;
		col = *((unsigned int *)c->texture[dt->tex_num].addr +(int) ((c->texture[dt->tex_num].height * dt->tex_y + dt->tex_x)));
		my_mlx_pixel_put(screen, x, y, col);
	}
}
void put_xpm_to_mlx_img(t_enemy *adv, double scale, int wid, int hei)
{
	int 			x, y = -1;
	int				pixel;
	unsigned int	col;

	(void)scale;
	while (++y < hei)//49 118
	{
		x = -1;
		while (++x < wid)
		{
			//printf("coucou %d %d ", x, y);
			pixel = (int)(adv->spr_fr->line_length * y / 4 + x * (adv->text_on.bits_per_pixel / 32));
			col = *((unsigned int *)adv->spr_fr->addr + pixel);
			//col = 0x00FF00FF;
			my_mlx_pixel_put(&adv->text_on, x, y, col);
		}
	}
}
/*
Need to fix : get old img width and height and prject to new
need to do : choose wich img to use depending to wich side player see
had done : copy img from xpm to mlx img
*/

void draw_enemy(t_cube *cube, t_point center, int x, int id)
{
	int			i;
	int			wid, hei;
	int			old_wid, old_hei;
	int			n_x, n_y;
	double		dist;
	double		scale = 0.0;
	//static int	nb_draw = 0;
	t_point		pos;
	t_enemy 	*adv;
	t_img_mlx	*img;

	i = -1;
	while (++i < cube->lvl->nb_enemy)
		if (cube->lvl->enemy[i].id == id)
			adv = &cube->lvl->enemy[i];
	pos = cube->player->pos;
	dist = dist_ab(pos, adv->pos);
	scale = dist / 20;
	(void)center;
	//printf("dist %f\n", dist);
	if (adv->text_on.img)
	{
		//printf("scale : %f %d %d\n", scale, wid, adv->spr_fr->width);
		img = adv->text_on.img;
		old_wid = img->width;
		old_hei = img->height;
		mlx_destroy_image(cube->mlx, adv->text_on.img);
	}
	else
	{
		img = adv->spr_fr->img;
		old_wid = img->width;
		old_hei = img->height;
	}
	
	wid = old_wid ;// * scale ;
	hei = old_hei ;// * scale ; // dont know how to make them look bigger without segfault

	cube->player->prev_pos = (t_point){cube->player->pos.x, cube->player->pos.y};
	n_x = x + wid / dist;
	dist = dist_ab(pos, adv->pos);
	// if (dist > 0 && dist < 1)
	// 	n_x = WIN_WIDTH / 2;
	n_y = 100 + (WIN_HEIGHT / 2) ;
	new_img(cube, &adv->text_on, wid, hei);
	put_xpm_to_mlx_img(adv, scale, wid, hei);
	
	mlx_put_image_to_window(cube->mlx, cube->win, adv->text_on.img, n_x, n_y);
}
