/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_enemy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:29:12 by Gfinet            #+#    #+#             */
/*   Updated: 2026/09/20 12:14:23 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void raycast_enemy(t_cube *cube)
{
	int      i;
    t_enemy  *adv;
    t_player *play;
	t_point play_pos, adv_pos;
    double   dx, dy, inv_det, cam_z;

    play = cube->player;
	pthread_mutex_lock(&cube->playpos_mutex);
	play_pos = play->pos;
	pthread_mutex_unlock(&cube->playpos_mutex);
    set_draw_enemy(cube, 0);
    i = -1;
    while (++i < cube->lvl->nb_enemy)
    {
        adv = &cube->lvl->enemies[i];
		
		pthread_mutex_lock(&adv->pos_mutex);
		adv_pos = adv->pos;
		pthread_mutex_unlock(&adv->pos_mutex);
        dx = adv_pos.x - play_pos.x;
        dy = adv_pos.y - play_pos.y;
        inv_det = 1.0 / (play->pov.x * play->dir.y - play->dir.x * play->pov.y);
        cam_z = inv_det * (-play->pov.y * dx + play->pov.x * dy);
        // ennemi devant le joueur et dans le frustum
        if (cam_z <= 0.0)
            continue ;
		adv->cam_z = cam_z;
		adv->cam_x = inv_det * (play->dir.y * dx - play->dir.x * dy);
        adv->short_dist = dist_ab(play->pos, adv->pos);
        adv->draw = 1;
    }
}


t_enemy *enemy_in_sight(t_cube *cube, t_rcdata *data)
{
	int		i;
	double	hit;
	t_enemy	*adv;
	t_point	posi, hitb, ray;

	adv = cube->lvl->enemies;
	ray = data->dest;
	i = 0;
	while (i < cube->lvl->nb_enemy)
	{
		posi = adv[i].pos;
		hitb = adv[i].hitbox;
		// hit = adv->bobox;
		(void)hitb;
		(void)hit;
		if ( \
		 (int)posi.x == (int)ray.x \
		&& (int)posi.y == (int)ray.y ) // && posi.y < (int)data->dest.x + 1) // && posi.x < (int)data->dest.x + 1 
		// ray.x < posi.x + hit / 2 && ray.x > posi.x - hit / 2 \
		// && ray.y < posi.y + hit / 2 && ray.y > posi.y - hit / 2 )
		// ray.x < posi.x + 0.5 && ray.x > posi.x - 0.5 \
		// && ray.y < posi.y + 0.5 && ray.y > posi.y - 0.5 )
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
			if (x <= (int)adv->st_dr_end.x || x >= (int)adv->st_dr_end.y)
				col = 0xFFFFFFFF;
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
		if (cube->lvl->enemies[i].id == id)
			adv = &cube->lvl->enemies[i];
	return (adv);
}
static int	get_en_side(t_enemy *adv, t_point adv_pos, t_point play_pos, t_data **text, int *max_text)
{
	t_point adv_dir;
	double	angle_to_player;
	double	enemy_angle;
	double	rel_angle;

	pthread_mutex_lock(&adv->dir_mutex);
	adv_dir = adv->dir;
	pthread_mutex_unlock(&adv->dir_mutex);

	// 1. Angle du vecteur Joueur -> Ennemi
	angle_to_player = atan2(play_pos.y - adv_pos.y, play_pos.x - adv_pos.x);
	// 2. Angle absolu où regarde l'ennemi
	enemy_angle = atan2(adv_dir.y, adv_dir.x);
	// 3. Différence d'angle ramenée entre -PI et PI
	rel_angle = angle_to_player - enemy_angle;
	while (rel_angle > M_PI)
		rel_angle -= 2 * M_PI;
	while (rel_angle < -M_PI)
		rel_angle += 2 * M_PI;

	// 4. Découpage en 4 secteurs d'angle (Front, Côté Droit, Dos, Côté Gauche)
	// rel_angle proche de 0 = L'ennemi regarde le joueur (FACE)
	// rel_angle proche de PI ou -PI = L'ennemi regarde ailleurs (DOS)
	if (rel_angle >= -M_PI_4 && rel_angle < M_PI_4)
	{
		*text = adv->type->spr_fr;
		*max_text = adv->type->max_text_fr;
		return (0); // Front
	}
	else if (rel_angle >= M_PI_4 && rel_angle < 3 * M_PI_4)
	{
		*text = adv->type->spr_sd; // Profil Droit
		*max_text = adv->type->max_text_sd;
		return (1); // Right Side
	}
	else if (rel_angle <= -M_PI_4 && rel_angle > -3 * M_PI_4)
	{
		*text = adv->type->spr_sd; // Profil Gauche (ou spr_sd_left si tu as un sprite miroir)
		*max_text = adv->type->max_text_sd;
		return (3); // Left Side
	}
	else
	{
		*text = adv->type->spr_bk;
		*max_text = adv->type->max_text_bk;
		return (2); // Back
	}
}

static void sort_enemies_by_dist(t_enemy *enemies, int nb)
{
    int     i;
    int     j;
    t_enemy tmp;

    i = 1;
    while (i < nb)
    {
        tmp = enemies[i];
        j = i - 1;
        // tri décroissant : le plus loin en premier
        while (j >= 0 && enemies[j].short_dist < tmp.short_dist)
        {
            enemies[j + 1] = enemies[j];
            j--;
        }
        enemies[j + 1] = tmp;
        i++;
    }
}

void draw_enemies(t_cube *cube)
{
	int		i = -1;
	t_enemy	*advs, *adv;

	advs = cube->lvl->enemies;
	// printf("nb en : %d", cube->lvl->nb_enemy);
	sort_enemies_by_dist(advs, cube->lvl->nb_enemy);
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

void compute_occlusion(t_enemy *adv, t_cube *cube, int sprite_left, int wid, int img_w, double cam_z)
{
	int     col;
    int     screen_col;
    // int     xpm_col;
    int     vis_start;
    int     vis_end;

    vis_start = -1;
    vis_end = -1;
    col = -1;
    while (++col < wid)
    {
        screen_col = sprite_left + col;
        // colonne hors écran : masquée
        if (screen_col < 0 || screen_col >= WIN_WIDTH)
            continue ;
        // si le mur est plus proche que l'ennemi : masqué
        if (cube->zbuffer[screen_col] < cam_z)
            continue ;
        // pixel visible : noter la première et dernière colonne
        if (vis_start == -1)
            vis_start = col;
        vis_end = col;
    }
    // aucun pixel visible
    if (vis_start == -1)
    {
        adv->st_dr_end.x = 0;
        adv->st_dr_end.y = 0;
        return ;
    }
    // convertir les bornes pixel écran en coordonnées XPM source
    adv->st_dr_end.x = (double)vis_start * img_w / wid;
    adv->st_dr_end.y = (double)(vis_end + 1) * img_w / wid;
}

void draw_enemy(t_cube *cube, t_enemy *adv)
{
	int			wid, hei, n_x, n_y, side = -1;
	int			max_text = 1;
	double		dist, scale = 0.0;
	double		cam_x, cam_z;
	int			screen_x;
	int 		z_offset;
	t_player	*play;
	t_point		play_pos, adv_pos;
	t_data		*use_text, *one_text;
	t_img_mlx	*img;

	play = cube->player;
	pthread_mutex_lock(&cube->playpos_mutex);
	play_pos = play->pos;
	pthread_mutex_unlock(&cube->playpos_mutex);
	pthread_mutex_lock(&adv->pos_mutex);
	adv_pos = adv->pos;
	pthread_mutex_unlock(&adv->pos_mutex);

	dist = dist_ab(play_pos, adv_pos);
	if (adv->short_dist <= 0.5)
    	return ;

	cam_z = adv->cam_z;
	cam_x = adv->cam_x;
	z_offset = (int)(play_pos.z / adv->short_dist);
	dist = adv->short_dist;
	screen_x = (int)((WIN_WIDTH / 2) * (1.0 + cam_x / cam_z));

	
	side = get_en_side(adv, adv_pos, play_pos, &use_text, &max_text);
	
	adv->fps++;
	if (adv->fps - 1 == (cube->frame / (1 + play->run) / 2))
		adv->nb_draw[side]++;
	adv->nb_draw[side] %= max_text;
	if (!adv->nb_draw[side])
		adv->nb_draw[side] = 1;

	pthread_mutex_lock(&adv->mov_mutex);
	one_text = &use_text[(adv->is_moving) * adv->nb_draw[side]];
	pthread_mutex_unlock(&adv->mov_mutex);
	img = one_text->img;
	
	scale = 6 / dist;
	adv->fps %= cube->frame * 4 + cube->frame * play->run;
	hei = img->height * scale;
	wid = img->width * scale;

	n_x = screen_x - wid / 2;
	n_y = WIN_HEIGHT / 2 - hei / 2 + 100 + play->z_view + z_offset;
	if (adv->text_on.img)
		mlx_destroy_image(cube->mlx, adv->text_on.img);
	new_img(cube, &adv->text_on, wid, hei);
	compute_occlusion(adv, cube, n_x, wid, img->width, cam_z);
	if (adv->st_dr_end.x == 0 && adv->st_dr_end.y == 0)
	{
		mlx_destroy_image(cube->mlx, adv->text_on.img);
		adv->text_on.img = NULL;
		return ;
	}
	put_xpm_to_mlx_img(adv, one_text, scale, (side == 1));
	mlx_put_image_to_window(cube->mlx, cube->win, adv->text_on.img, n_x, n_y);
}

