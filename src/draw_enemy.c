/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_enemy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:29:12 by Gfinet            #+#    #+#             */
/*   Updated: 2024/11/10 19:16:42 by Gfinet           ###   ########.fr       */
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
			draw_enemy(cube, x, en_id);
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
			if ((x) * scale + l < img->width && (y) * scale + m < img->height)
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
			x = img->width + 1;
			while (--x)
			{
				pixel = (int)(use_text->line_length * y / 4 + x * (use_text->bits_per_pixel / 32));
				col = *((unsigned int *)use_text->addr + pixel);
				
				if (scale > 1)
					up_scale(adv, img->width - x, y, scale, col);		
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

void draw_enemy(t_cube *cube, int x, int id)
{
	int			wid, hei, n_x, n_y, side = -1;
	int			max_text;
	double		dist, scale = 0.0;
	static int	nb_draw[4] = {0, 0, 0, 0},	fps = 0;
	t_point		pos;
	t_data		*use_text;
	t_enemy 	*adv;
	t_img_mlx	*img;

	pos = cube->player->pos;
	adv = get_enemy(cube, id);
	dist = dist_ab(pos, adv->pos);
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
	hei = img->height * scale;
	wid = img->width * scale;
	n_x = x - wid / 2;
	n_y = cube->ground_end + (WIN_HEIGHT - cube->ground_end) / dist - hei;
	if (adv->text_on.img)
		mlx_destroy_image(cube->mlx, adv->text_on.img);
	if(!adv->text_on.img)
		new_img(cube, &adv->text_on, wid, hei);
	new_img(cube, &adv->text_on, wid, hei);
	put_xpm_to_mlx_img(adv, &use_text[nb_draw[side]], scale, (side == 1));
	mlx_put_image_to_window(cube->mlx, cube->win, adv->text_on.img, n_x, n_y);
}
