/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:02:38 by Gfinet            #+#    #+#             */
/*   Updated: 2026/09/03 17:17:37 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

static int has_seen_player(t_enemy *adv)
{
	t_point play_pos;
    double  dx, dy, dist, dot;

    play_pos = adv->cube->player->pos;
    dx = play_pos.x - adv->pos.x;
    dy = play_pos.y - adv->pos.y;
    dist = sqrt(dx * dx + dy * dy);
    // if (dist > 5.0)
    //     return (0);
    if (dist < 0.0001)
        return (1);
    dot = adv->dir.x * (dx / dist) + adv->dir.y * (dy / dist);
    return (dot >= 0.707);
}

// static t_point compute_cam(t_enemy *adv)
// {
// 	double		dx, dy, inv_det;
// 	t_point		pos;
// 	t_point		cam;

// 	pos = adv->cube->player->pos;
// 	dx = pos.x - adv->pos.x;
// 	dy = pos.y - adv->pos.y;
// 	inv_det = 1.0 / (adv->dir.x * adv->dir.y - adv->dir.x * adv->dir.y);
// 	cam.x = inv_det * (adv->dir.y * dx - adv->dir.x * dy);
// 	cam.y = inv_det * (-adv->dir.y * dx + adv->dir.x * dy);
// 	return cam;
// }

static void look_to_player(t_enemy *adv)
{
	// t_point	cam;

	// cam = compute_cam(adv);

	t_point play_pos = adv->cube->player->pos;

    double dx = play_pos.x - adv->pos.x;
    double dy = play_pos.y - adv->pos.y;

    double dist = sqrt(dx * dx + dy * dy);

    if (dist < 0.0001)
        return;
    adv->dir.x = dx / dist;
    adv->dir.y = dy / dist;

}



static void enemy_move(t_enemy *adv)
{
	t_cube	*cb;
	t_point	n_pos;
	t_point play_pos;
	double dx;
    double dy;
	// double dist;

	// double	n_x;
	// double	n_y;
	// double	rad;

	play_pos = adv->cube->player->pos;
	dy = play_pos.y - adv->pos.y;
	dx = play_pos.x - adv->pos.x;
	// dist = sqrt(dx * dx + dy * dy);

	cb = adv->cube;
	adv->prev_pos = adv->pos;
	adv->play_seen = has_seen_player(adv);
	// if (adv->play_seen)
		look_to_player(adv);

	// rad = (90) * (M_PI / 180.0);
	adv->prev_pos = (t_point){adv->pos.x, adv->pos.y};
	// n_x = (adv->dir.x * cos(-rad)) - (adv->dir.y) * sin(-rad);
	// n_y = adv->dir.x * sin(-rad) + (adv->dir.y) * cos(-rad);
	n_pos.x = adv->pos.x + adv->speed * (adv->dir.x / ((4 * cb->frame)));
	n_pos.y = adv->pos.y + adv->speed * (adv->dir.y / ((4 * cb->frame)));
	// n_pos.x += (n_x / (4 * cb->frame));
	// n_pos.y += (n_y / (4 * cb->frame));

	if (!impassable(cb->lvl->c_maps, cb, n_pos.x, adv->pos.y) && (fabs(dx) > 1.0))
		adv->pos.x = n_pos.x;
	if (!impassable(cb->lvl->c_maps, cb, adv->pos.x, n_pos.y) && (fabs(dy) > 1.0))
		adv->pos.y = n_pos.y;
}

void *enemy_thread(void *data)
{
	t_enemy *adv;
	t_cube	*cube;

	adv = (t_enemy *)data;
	cube = adv->cube;
	printf("bf move %d\n", cube->stop);
	while (!cube->stop)
	{
		// printf("pos %f %f - %f %f", adv->pos.x, adv->pos.y, cube->player->pos.x, cube->player->pos.y);
		enemy_move(adv);
	}
	return 0;
}


int launch_eneny_thread(t_cube *cube)
{
	t_lvl *lvl;

	lvl = cube->lvl;
	for (int i = 0; i< lvl->nb_enemy; i++)
	{
		pthread_create(&lvl->enemies[i].thread, 0, enemy_thread, &lvl->enemies[i]);
	}
	return 1;
}
