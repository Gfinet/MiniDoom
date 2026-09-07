/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:02:38 by Gfinet            #+#    #+#             */
/*   Updated: 2026/09/07 21:50:38 by Gfinet           ###   ########.fr       */
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

static int keep_space(t_enemy *adv, t_point n_pos)
{
	t_cube	*cube;
	t_lvl	*lvl;
	t_enemy *other;
	t_point play_pos;
    double dx, dy;
	double size_x, size_y;

	cube = adv->cube;
	lvl = cube->lvl;
	play_pos = adv->cube->player->pos;
	dx = play_pos.x - n_pos.x;
	dy = play_pos.y - n_pos.y;
	size_x = 0.5 + (adv->hitbox.x / 2);
	size_y = 0.5 + (adv->hitbox.y / 2);
	if (fabs(dy) < size_y && fabs(dx) < size_x)
		return 0;
	for (int i=0; i < lvl->nb_enemy; i++)
	{
		other = &lvl->enemies[i];
		if (adv == other)
			continue;
		dx = other->pos.x - n_pos.x;
		dy = other->pos.y - n_pos.y;
		size_x = 0.5 + (adv->hitbox.x / 2);
		size_y = 0.5 + (adv->hitbox.y / 2);
		if (fabs(dy) < size_y && fabs(dx) < size_x)
			return 0;
	}
	return 1;
}

static void enemy_move(t_enemy *adv)
{
	t_cube	*cb;
	t_point	n_pos, n_pos_x, n_pos_y;

	cb = adv->cube;
	// adv->prev_pos = adv->pos;
	adv->play_seen = has_seen_player(adv);
	if (adv->play_seen)
		look_to_player(adv);
	else
		return ;
	// adv->prev_pos = (t_point){adv->pos.x, adv->pos.y};
	n_pos.x = adv->pos.x + adv->speed * (adv->dir.x / ((4 * cb->frame)));
	n_pos.y = adv->pos.y + adv->speed * (adv->dir.y / ((4 * cb->frame)));
	n_pos_x = (t_point){n_pos.x, adv->pos.y};
	n_pos_y = (t_point){adv->pos.x, n_pos.y};
	
	if (!impassable(cb->lvl->c_maps, cb, n_pos.x, adv->pos.y) && keep_space(adv, n_pos_x))
		adv->pos.x = n_pos.x;
	if (!impassable(cb->lvl->c_maps, cb, adv->pos.x, n_pos.y) && keep_space(adv, n_pos_y))
		adv->pos.y = n_pos.y;
}

void *enemy_thread(void *data)
{
	int stop, move;
	t_enemy *adv;
	t_cube	*cube;

	adv = (t_enemy *)data;
	cube = adv->cube;
	stop = cube->stop;
	move = !cube->pause;
	printf("bf move %d\n", cube->stop);
	while (!stop)
	{
		// printf("pos %f %f - %f %f", adv->pos.x, adv->pos.y, cube->player->pos.x, cube->player->pos.y);
		if (move)
			enemy_move(adv);
		// printf("Stop: %d\n", cube->stop);
		usleep(1000000 / 60);
		stop = cube->stop;
		move = !cube->pause;
	}
	printf("Thread %d Stop\n", adv->id);
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
