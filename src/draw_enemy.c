/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_enemy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:29:12 by Gfinet            #+#    #+#             */
/*   Updated: 2024/11/20 01:24:25 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

// static double	ray_hit(t_point ray, t_point var, int side)
// {
// 	if (side % 2)
// 		return (ray.x - var.x);
// 	else
// 		return (ray.y - var.y);
// }

void raycast_enemy(t_cube *cube)
{
	int			x, en_seen;
	t_ray_hit	*hit_data;
	t_rcdata	data;
	t_drawdata	dr;
	t_enemy		*adv = 0;
	t_point		*play_pos;
	
	play_pos = &cube->player->pos;
	cube->player->pov.x = -(float)(0.66 * cube->player->dir.y);
	cube->player->pov.y = (float)(0.66 * cube->player->dir.x);
	x = -1;
	set_draw_enemy(cube, 0);
	hit_data = &cube->hit_data;
	//printf("%p %p\n", hit_data->enemies_hit, hit_data->enemies_dist);
	while (++x < WIN_WIDTH)
	{
		en_seen = 0;
		hit_data->wall_dist = -1;
		hit_data->nb_enemies = 0;
		data.camerx = 2 * x / ((double)WIN_WIDTH) - 1; //x-coordinate in camera space
		data.rays.x = cube->player->dir.x + cube->player->pov.x * data.camerx;
		data.rays.y = cube->player->dir.y + cube->player->pov.y * data.camerx;
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
		while (in_char_lst(data.hit, INVIS_WALL)) //ray_in_limit(cube, data.dest.x, data.dest.y))
		{
			// if (data.hit == '1' && hit_data->wall_dist == -1)
			// 	hit_data->wall_dist = ray_hit(data.side_dist, data.var, data.side);
			adv = enemy_in_sight(cube, &data);
			if (adv)
			{
				adv->tmp_dist = dist_ab(*play_pos, adv->pos);
				// hit_data->enemies_hit[hit_data->nb_enemies] = adv;
				// hit_data->enemies_dist[hit_data->nb_enemies] = \
				// 	ray_hit(data.side_dist, data.var, data.side);
				//hit_data->nb_enemies++;
				if (adv->tmp_dist < adv->short_dist)
				{
					adv->short_dist = adv->tmp_dist;
					adjust_enemy_visibility(cube, adv, &data);
					adv->x = x;
					//adv->wall_dist = adv->tmp_dist;
					adv->draw++;
					en_seen = 1;
				}
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
			//if (ray_in_limit(cube, data.dest.x, data.dest.y))
			data.hit = cube->lvl->c_maps[(int)data.dest.y][(int)data.dest.x];
		}
		if (en_seen)
		{
			//data.perp_wall_dist = ray_hit(data.side_dist, data.var, data.side);
			if (data.side % 2)
				data.perp_wall_dist = (data.side_dist.y - data.var.y);
			else
				data.perp_wall_dist = (data.side_dist.x - data.var.x);
			
			dr.draw_start = WIN_HEIGHT / 2 - dr.line_height / 2 + 100;
			if (dr.draw_start < 0)
				dr.draw_start = 0;
			dr.line_height = (int)( WIN_HEIGHT / data.perp_wall_dist);
			dr.draw_end = - dr.line_height / 2 + WIN_HEIGHT / 2 + 100;
			if (dr.draw_end >= WIN_HEIGHT)
				dr.draw_end = WIN_HEIGHT - 1;
			cube->wall = dr.draw_end - dr.draw_start;
			cube->wall /=2;
			set_enemies_seen(cube, x, data.perp_wall_dist);
		}
	}
}

void set_enemies_seen(t_cube *cube, int x, double wall_dist)
{
	int			i = -1;
	t_enemy *advs, *adv;

	advs = cube->lvl->enemy;
	while (++i < cube->lvl->nb_enemy)
	{
		adv = &advs[i];
		if (adv->x == x && adv->draw == 1)
			adv->wall_dist = wall_dist;
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
	while (i < cube->lvl->nb_enemy)
	{
		posi = adv[i].pos;
		hitb = adv[i].hitbox;
		if (!(adv[i].draw) \
		// && (int)posi.x == (int)ray.x \
		// && (int)posi.y == (int)ray.y ) // && posi.y < (int)data->dest.x + 1) // && posi.x < (int)data->dest.x + 1 
		&& ray.x < posi.x + hitb.x / 2 && ray.x > posi.x - hitb.x / 2 \
		&& ray.y < posi.y + hitb.y / 2 && ray.y > posi.y - hitb.y / 2 )
			return (&adv[i]);
		i++;
	}
	return (0);
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

void adjust_enemy_visibility(t_cube *cube, t_enemy *adv, t_rcdata *data)
{
	double hitbox_left, hitbox_right;
    double intersection_start, intersection_end;

    // Calculer les bords gauche et droit de la hitbox de l'ennemi
    hitbox_left = adv->pos.x - adv->hitbox.x / 2;
    hitbox_right = adv->pos.x + adv->hitbox.x / 2;

    // Calculer les intersections entre le rayon et les bords de la hitbox
    intersection_start = (hitbox_left - cube->player->pos.x) / data->rays.x;
    intersection_end = (hitbox_right - cube->player->pos.x) / data->rays.x;

    // Convertir les intersections en coordonnées écran (X)
    adv->st_dr_end.x = (int)((WIN_WIDTH / 2) * (1 + intersection_start / data->perp_wall_dist));
    adv->st_dr_end.y = (int)((WIN_WIDTH / 2) * (1 + intersection_end / data->perp_wall_dist));

    // Clamper les valeurs pour rester dans les limites de l'écran
    if (adv->st_dr_end.x < 0) adv->st_dr_end.x = 0;
    if (adv->st_dr_end.y >= WIN_WIDTH) adv->st_dr_end.y = WIN_WIDTH - 1;	
}

void put_xpm_to_mlx_img(t_enemy *adv, t_data *use_text, double scale, int side)
{
	int 			x, y = -1, xx;
	unsigned int	col;
	t_img_mlx		*img, *img2;

	img = use_text->img;
	img2 = adv->text_on.img;
	while (++y < img->height)
	{
		x = -1;
		while (++x < img->width)
		{
			col = get_color_from_xpm(use_text, x, y);
			// if (x < adv->st_dr_end.x || x > adv->st_dr_end.y)
			// 	col = 0xFFFFFFFF;
			if (!side)
				xx = x;
			else
				xx = img->width - x;
			if (scale > 1)
				up_scale(adv, xx - side, y, scale, col);
			else if (!(xx % (int)(1 / scale)) && !(y % (int)(1 / scale)))
				if (xx * scale < img2->width && y * scale < img2->height)
					my_mlx_pixel_put(&adv->text_on, (xx * scale), (y * scale), col);
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
	int			wid, hei, n_x, n_y, side = -1; //, fix;
	int			max_text;
	double		dist, dist_w, scale = 0.0;
	static int	nb_draw[4] = {0, 0, 0, 0},	fps = 0;
	t_player	*play;
	t_point		pos;
	t_data		*use_text;
	t_img_mlx	*img;

	play = cube->player;
	pos = play->pos;
	
	dist = dist_ab(pos, adv->pos);
	dist_w = adv->wall_dist;
	if (dist <= 0)
		return ;
	scale = 6 / dist;
	
	side = get_en_side(adv, play->dir, &use_text, &max_text);
	fps++;
	if (fps - 1 == (cube->frame / (1 + play->run) / 2))
		nb_draw[side]++;
	nb_draw[side] %= max_text;
	img = use_text[nb_draw[side]].img;
	fps %= cube->frame * 4 + cube->frame * play->run;

	// if (side % 2)
	// 	fix = (adv->pos.y - adv->dest.y) / adv->hitbox.y;
	// else
	// 	fix = (adv->pos.x - adv->dest.x) / adv->hitbox.x;
	hei = img->height * scale;
	wid = img->width * scale;
	n_x = adv->x;
	//n_y = adv->ground_end - cube->wall + (dist) / dist_w ; //(adv->ground_end) * (dist) / (dist_w);
	//n_y = (int)(WIN_HEIGHT / dist);
	n_y = WIN_HEIGHT / 2 - hei / 2 + 110;
	//printf("%f\n", adv->ground_end / dist_w);
	//printf("%d %d ", cube->ground_end, n_y);
	(void)dist_w;
	if (adv->text_on.img)
		mlx_destroy_image(cube->mlx, adv->text_on.img);
	new_img(cube, &adv->text_on, wid, hei);
	put_xpm_to_mlx_img(adv, &use_text[nb_draw[side]], scale, (side == 1));
	mlx_put_image_to_window(cube->mlx, cube->win, adv->text_on.img, n_x, n_y);
}

