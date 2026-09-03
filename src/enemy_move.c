/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:02:38 by Gfinet            #+#    #+#             */
/*   Updated: 2026/09/03 16:07:33 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

int has_seen_player(t_enemy *adv)
{
	t_point 	vision_plan;
	t_point 	play_pos;
	int 		in_vision;
	int 		close = (adv->short_dist <= 5);

	play_pos = adv->cube->player->pos;
	vision_plan = {adv->pos.x + 5 * adv->dir.x, adv->pos.y + 5 * adv->dir.y};
	
	return ((play_pos.x > adv->pos.x) && (adv->pos.x > vision_plan.x) ||
			(play_pos.x > adv->pos.x) && (adv->pos.x > vision_plan.x)) &&
			((play_pos.y > adv->pos.y) && (adv->pos.y > vision_plan.y) ||
			(play_pos.y > adv->pos.y) && (adv->pos.y > vision_plan.y))
}

t_point compute_cam(t_enemy *adv)
{
	double		dx, dy, inv_det;
	t_point		pos;
	t_point		cam;

	pos = adv->cube->player->pos;
	dx = pos.x - adv->pos.x;
	dy = pos.y - adv->pos.y;
	inv_det = 1.0 / (adv->dir.x * adv->dir.y - adv->dir.x * adv->dir.y);
	cam.x = inv_det * (adv->dir.y * dx - adv->dir.x * dy);
	cam.y = inv_det * (-adv->dir.y * dx + adv->dir.x * dy);
	return cam;
}

void move_to_player(t_enemy *adv)
{
	// t_point	cam;

	// cam = compute_cam(adv);

	t_point play_pos = adv->cube->player->pos;

    double dx = play_pos.x - adv->pos.x;
    double dy = play_pos.y - adv->pos.y;

    double dist = adv->short_dist;

    if (dist < 0.0001)
        return;
    adv->dir.x = dx / dist;
    adv->dir.y = dy / dist;

}

void enemy_thread(t_enemy *adv)
{
	while (!adv->cube->stop)
	{
		enemy_move(adv);
	}
}

void enemy_move(t_enemy *adv)
{
	t_point	n_pos;
	double	n_x;
	double	n_y;
	double	rad;

	adv->prev_pos = adv->pos;
	adv->enemy->play_seen = has_seen_player(adv);
	if (adv->enemy->play_seen)
		move_to_player(adv);

	rad = (90) * (M_PI / 180.0);
	adv->prev_pos = (t_point){adv->pos.x, adv->pos.y};
	n_x = (adv->dir.x * cos(-rad)) - (adv->dir.y) * sin(-rad);
	n_y = adv->dir.x * sin(-rad) + (adv->dir.y) * cos(-rad);
	n_pos.x = adv->pos.x + adv->move_v * (adv->dir.x / (4 * cb->frame));
	n_pos.y = adv->pos.y + adv->move_v * (adv->dir.y / (4 * cb->frame));
	n_pos.x += adv->move_h * (n_x / (4 * cb->frame));
	n_pos.y += adv->move_h * (n_y / (4 * cb->frame));

	if (!impassable(cb->lvl->c_maps, cb, n_pos.x, adv->pos.y))
		adv->pos.x = n_pos.x;
	if (!impassable(cb->lvl->c_maps, cb, adv->pos.x, n_pos.y))
		adv->pos.y = n_pos.y;
}


int launch_eneny_thread(t_cube *cube)
{
	t_maps *lvl;

	lvl = cube->lvl;
	for (int i = 0; i< lvl->nb_enemy; i++)

}