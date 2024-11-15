/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_enemy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:29:12 by Gfinet            #+#    #+#             */
/*   Updated: 2024/11/14 21:45:12 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void raycast_enemy(t_cube *cube)
{
	int			x, en_seen;
	t_rcdata	data;
	t_drawdata	dr;
	t_enemy		*adv = 0;
	t_point		*play_pos;
	
	play_pos = &cube->player->pos;
	data.pov.x = -(float)(0.66 * cube->player->dir.y);
	data.pov.y = (float)(0.66 * cube->player->dir.x);
	x = -1;
	set_draw_enemy(cube, 0);
	while (++x < WIN_WIDTH)
	{
		en_seen = 0;
		data.camerx = 2 * x / ((double)WIN_WIDTH) - 1; //x-coordinate in camera space
		data.rays.x = cube->player->dir.x + data.pov.x * data.camerx;
		data.rays.y = cube->player->dir.y + data.pov.y * data.camerx;
		//Digital Differential Analysis
		data.var.x = set_delta(data.rays.x);
		data.var.y = set_delta(data.rays.y);
		
		data.dest.x = (int)play_pos->x;
		data.dest.y = (int)play_pos->y;
		
		data.step.x = (!(data.rays.x < 0) - (data.rays.x < 0)) * 0.5;
		data.step.y = (!(data.rays.y < 0) - (data.rays.y < 0)) * 0.5;
		if (data.step.x == -0.5)
			data.side_dist.x = (play_pos->x - data.dest.x) * data.var.x;
		else
			data.side_dist.x = (1.0 + data.dest.x - play_pos->x) * data.var.x;
		if (data.step.y == -0.5)
			data.side_dist.y = (play_pos->y - data.dest.y) * data.var.y;
		else
			data.side_dist.y = (1.0 + data.dest.y - play_pos->y) * data.var.y;

		
		data.hit = '0';
		while (in_char_lst(data.hit, INVIS_WALL))
		{
			adv = enemy_in_sight(cube, &data);
			if (adv && !adv->draw)
			{
				adv->pix_pos = data.dest;
				adv->x = x;
				adv->draw = 1;
				en_seen = 1;
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
		if (en_seen)
		{
			if (data.side % 2)
				data.perp_wall_dist = (data.side_dist.y - data.var.y);
			else
				data.perp_wall_dist = (data.side_dist.x - data.var.x);
			
			dr.line_height = (int)( WIN_HEIGHT / data.perp_wall_dist);
			dr.draw_end = - dr.line_height / 2 + WIN_HEIGHT / 2 + 100;
			if (dr.draw_end >= WIN_HEIGHT)
				dr.draw_end = WIN_HEIGHT - 1;

			set_enemies_seen(cube, x, data.perp_wall_dist, dr.draw_end);
		}
	}
}

void set_enemies_seen(t_cube *cube, int x, double wall_dist, int dr_end)
{
	int			i = -1;
	t_enemy *advs, *adv;

	advs = cube->lvl->enemy;
	while (++i < cube->lvl->nb_enemy)
	{
		adv = &advs[i];
		if (adv->x == x && adv->draw)
		{
			adv->ground_end = dr_end;
			adv->wall_dist = wall_dist;
		}
	}
}

t_enemy *enemy_in_sight(t_cube *cube, t_rcdata *data)
{
	int		i;
	t_enemy	*adv;
	t_point	posi, hitb, ray;

	adv = cube->lvl->enemy;
	ray = data->dest;
	i = 0;
	(void)hitb;
	while (i < cube->lvl->nb_enemy)
	{
		posi = adv[i].pos;
		hitb = adv[i].hitbox;
		if (!(adv[i].draw) \
		// && (int)posi.x == (int)ray.x \
		// && (int)posi.y == (int)ray.y ) // && posi.y < (int)data->dest.x + 1) // && posi.x < (int)data->dest.x + 1 
		&& ray.x < posi.x + hitb.x && ray.x > posi.x - hitb.x \
		&& ray.y < posi.y + hitb.y && ray.y > posi.y - hitb.y )
			return (adv->dest = ray, &adv[i]);
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

void up_scale(t_enemy *adv, int x, int y, double scale, unsigned int col)
{
	int			l, m;
	t_img_mlx	*img;

	l = -1;
	img = adv->text_on.img;
	while (++l <= scale)
	{
		m = -1;
		while (++m <= scale)
		{
			//printf("bug? %d %d %d %d\n", (int)((x) * scale) + l, (int)(y * scale) + m, img->width, img->height);
			if ((int)((x) * scale) + l < img->width && (int)(y * scale) + m < img->height)
				my_mlx_pixel_put(&adv->text_on, (int)((x) * scale) + l, (int)(y * scale) + m, col);
			// else
			// 	printf("%d %d %d %d\n", (int) (x * scale) + l, img->width, (int) (y * scale) + m, img->height);
		}
	}
}

void put_xpm_to_mlx_img(t_enemy *adv, t_data *use_text, double scale, int side)
{
	int 			x, y = -1;
	int				pixel;
	unsigned int	col;
	t_img_mlx		*img, *img2;

	img = use_text->img;
	img2 = adv->text_on.img;
	while (++y < img->height)
	{
		if (!side)
		{
			x = -1;
			while (++x < img->width)
			{
				pixel = (int)(use_text->line_length * y / 4 + x * (use_text->bits_per_pixel / 32));
				col = *((unsigned int *)use_text->addr + pixel);
				
				if (scale > 1)
					up_scale(adv, x, y, scale, col);		
				else if (!(x % (int)(1 / scale)) && !(y % (int)(1 / scale)))
				{
					if (x * scale < img2->width && y * scale < img2->height)
						my_mlx_pixel_put(&adv->text_on, (x * scale), (y * scale), col);
				}
			}
		}
		else
		{
			x = -1;
			while (++x < img->width)
			{
				pixel = (int)(use_text->line_length * y / 4 + x * (use_text->bits_per_pixel / 32));
				col = *((unsigned int *)use_text->addr + pixel);
				
				if (scale > 1)
					up_scale(adv, img->width - 1 - x, y, scale, col);		
				else if (!(x % (int)(1 / scale)) && !(y % (int)(1 / scale)))
				{
					if ((img->width - x) * scale < img2->width && y * scale < img2->height)
						my_mlx_pixel_put(&adv->text_on, ((img->width - x) * scale), (y * scale), col);
				}
			}
		}
	}
}

t_enemy *get_enemy(t_cube *cube, int id)
{
	int		i;
	t_enemy	*adv;

	i = -1;
	while (++i < cube->lvl->nb_enemy)
		if (cube->lvl->enemy[i].id == id)
			adv = &cube->lvl->enemy[i];
	return (adv);
}

double get_angle_vect(t_point adv, t_point play)
{
	double	angle, res, up_form, down_form, sqrt_x, sqrt_y;

	up_form = (play.x * adv.x + play.y * adv.y);
	sqrt_x = sqrt(play.x * play.x + adv.x * adv.x);
	sqrt_y = sqrt(play.y * play.y + adv.y * adv.y);
	down_form = sqrt_x * sqrt_y;
	res = up_form / down_form;
	angle = acos(res);
	printf("%f %f %f %f %f %f\n",up_form, down_form, sqrt_x, sqrt_y, res, angle);
	return (angle);
}

int get_en_side(t_enemy *adv, t_point play_dir, t_data **text, int *max_text)
{
	//double	cosi, sini;
	int		side = 0;
	t_point diff;

	diff.x = play_dir.x + adv->dir.x;
	diff.y = play_dir.y + adv->dir.y;
	if (diff.x >= 0.707)
	{
		side = 1;
		*text = adv->spr_sd;
		*max_text = adv->max_text_sd;
		//printf("side droite\n");
	}
	else if (diff.x < 0.707 && diff.x > -0.707)
	{
		if (diff.y > -1)
		{
			side = 2;
			*text = adv->spr_bk;
			*max_text = adv->max_text_bk;
			//printf("back\n");
		}
		else 
		{
			side = 0;
			*max_text = adv->max_text_fr;
			*text = adv->spr_fr;
			//printf("front\n");
		}
	}
	else
	{
		side = 3;
		*text = adv->spr_sd;
		*max_text = adv->max_text_sd;
		//printf("side gauche\n");
	}
	
	return (side);
}

void draw_enemies(t_cube *cube)
{
	int		i = -1;
	t_enemy	*advs, *adv;

	advs = cube->lvl->enemy;
	while (++i < cube->lvl->nb_enemy)
	{
		adv = &advs[i];
		if (adv->draw)
		{
			draw_enemy(cube, adv);
			adv->draw = 0;
		}
	}
}


void draw_enemy(t_cube *cube, t_enemy *adv)
{
	int			wid, hei, n_x, n_y, side = -1, fix;
	int			max_text;
	double		dist, dist_w, scale = 0.0;
	static int	nb_draw[4] = {0, 0, 0, 0},	fps = 0;
	t_point		pos;
	t_data		*use_text;
	t_img_mlx	*img;

	pos = cube->player->pos;
	dist = dist_ab(pos, adv->pos);
	dist_w = adv->wall_dist;
	if (dist == 0)
		return ;
	side = get_en_side(adv, cube->player->dir, &use_text, &max_text);
	scale = 6 / dist;
	fps++;
	if (fps - 1 == (cube->frame / (1 + cube->player->run) / 2))
		nb_draw[side]++;
	nb_draw[side] %= max_text;
	img = use_text[nb_draw[side]].img;
	fps %= cube->frame * 4 + cube->frame * cube->player->run;

	if (side % 2)
		fix = (adv->pos.y - adv->dest.y) / adv->hitbox.y;
	else
		fix = (adv->pos.x - adv->dest.x) / adv->hitbox.x;
	hei = img->height * scale;
	wid = img->width * scale;
	n_x = adv->x - fix;
	n_y = adv->ground_end + dist / dist_w ; //(adv->ground_end) * (dist) / (dist_w);
	printf("%f\n", adv->ground_end / dist_w);
	(void)dist_w;
	(void)n_x;
	(void)n_y;
	//n_y = cube->ground_end + (WIN_HEIGHT - cube->ground_end) / dist - hei;
	//printf("%d %d ", cube->ground_end, n_y);
	if (adv->text_on.img)
		mlx_destroy_image(cube->mlx, adv->text_on.img);
	new_img(cube, &adv->text_on, wid, hei);
	put_xpm_to_mlx_img(adv, &use_text[nb_draw[side]], scale, (side == 1));
	mlx_put_image_to_window(cube->mlx, cube->win, adv->text_on.img, n_x, n_y);
}
