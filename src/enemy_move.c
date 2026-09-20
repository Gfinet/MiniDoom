/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:02:38 by Gfinet            #+#    #+#             */
/*   Updated: 2026/09/20 02:19:12 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

static int en_impassable(char **map, double x, double y)
{
    int map_x = (int)x;
    int map_y = (int)y;

    // Sécurité pour éviter un Segfault hors-map
    if (map_y < 0 || map_x < 0 || !map[map_y] || !map[map_y][map_x])
        return (1);

    if (map[map_y][map_x] == '1' || map[map_y][map_x] == '2')
        return (1);
    return (0);
}

static int wall_between(t_enemy *adv)
{
	t_point visu, play_pos;
	t_lvl *lvl;
	double total_dist, traveled = 0;
	double dx;
    double dy;

	pthread_mutex_lock(&adv->cube->playpos_mutex);
	play_pos = adv->cube->player->pos;
	pthread_mutex_unlock(&adv->cube->playpos_mutex);
	pthread_mutex_lock(&adv->pos_mutex);
	visu = adv->pos;
	pthread_mutex_unlock(&adv->pos_mutex);
	dx = play_pos.x - visu.x;
	dy= play_pos.y - visu.y;
	total_dist = sqrt(dx * dx + dy * dy);
	if (total_dist < 0.0001)
        return 0;
    dx /= total_dist;
    dy /= total_dist;

	lvl = adv->cube->lvl;
	visu = adv->pos;
	while (traveled < total_dist - 0.2)
	{
		visu.x += dx * 0.1;
		visu.y += dy * 0.1;
		if (en_impassable(lvl->c_maps, visu.x, visu.y))
			return 1;

		traveled += 0.1;
	}
	return 0;
}

static int see_player(t_enemy *adv)
{
	t_point play_pos, adv_pos;
    double  dx, dy, dist, dot;

	pthread_mutex_lock(&adv->cube->playpos_mutex);
    play_pos = adv->cube->player->pos;
	pthread_mutex_unlock(&adv->cube->playpos_mutex);
	pthread_mutex_lock(&adv->pos_mutex);
    adv_pos = adv->pos;
	pthread_mutex_unlock(&adv->pos_mutex);
    dx = play_pos.x - adv_pos.x;
    dy = play_pos.y - adv_pos.y;
    dist = sqrt(dx * dx + dy * dy);
	if (wall_between(adv))
		return 0;
    // if (dist > 5.0)
    //     return (0);
    if (dist < 0.0001)
        return (1);
    dot = adv->dir.x * (dx / dist) + adv->dir.y * (dy / dist);
    return (dot >= 0.707);
}

static void look_to_player(t_enemy *adv)
{
	pthread_mutex_lock(&adv->cube->playpos_mutex);
	t_point play_pos = adv->cube->player->pos;
	pthread_mutex_unlock(&adv->cube->playpos_mutex);

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
	t_point play_pos, hitb;
    double dx, dy;
	double size_x, size_y;

	cube = adv->cube;
	lvl = cube->lvl;

	pthread_mutex_lock(&adv->cube->playpos_mutex);
	play_pos = adv->cube->player->pos;
	pthread_mutex_unlock(&adv->cube->playpos_mutex);
	dx = play_pos.x - n_pos.x;
	dy = play_pos.y - n_pos.y;
	hitb.x = adv->hitbox.x / 2;
	hitb.y = adv->hitbox.y / 2;
	size_x = 0.5 + hitb.x;
	size_y = 0.5 + hitb.y;
	if (fabs(dy) < size_y && fabs(dx) < size_x)
		return 0;
	for (int i=0; i < lvl->nb_enemy; i++)
	{
		other = &lvl->enemies[i];
		if (adv == other)
			continue;
		pthread_mutex_lock(&other->pos_mutex);
		dx = other->pos.x - n_pos.x;
		dy = other->pos.y - n_pos.y;
		pthread_mutex_unlock(&other->pos_mutex);
		size_x = (other->hitbox.x / 2.0) + (adv->hitbox.x / 2.0);
		size_y = (other->hitbox.y / 2.0) + (adv->hitbox.y / 2.0);
		if (fabs(dy) < size_y && fabs(dx) < size_x)
			return 0;
	}
	hitb.x *= (-1) * (adv->dir.x < 0);
	hitb.y *= (-1) * (adv->dir.y < 0);
	return !impassable(cube->lvl->c_maps, n_pos.x, n_pos.y);
}

static void move_forward(t_enemy *adv)
{
	t_point	n_pos, n_pos_x, n_pos_y;
	t_cube	*cb;

	cb = adv->cube;
	
	n_pos.x = adv->pos.x + adv->speed * (adv->dir.x / ((4 * cb->frame)));
	n_pos.y = adv->pos.y + adv->speed * (adv->dir.y / ((4 * cb->frame)));
	n_pos_x = (t_point){n_pos.x, adv->pos.y, adv->pos.z};
	n_pos_y = (t_point){adv->pos.x, n_pos.y, adv->pos.z};
	
	
	if (keep_space(adv, n_pos_x))
	{
		pthread_mutex_lock(&adv->pos_mutex);
		adv->pos.x = n_pos.x;
		pthread_mutex_unlock(&adv->pos_mutex);
	}
	if (keep_space(adv, n_pos_y))
	{
		pthread_mutex_lock(&adv->pos_mutex);
		adv->pos.y = n_pos.y;
		pthread_mutex_unlock(&adv->pos_mutex);
	}
}

static void turn_face(t_enemy *adv, int left_right)
{
	double		n_x;
	double		n_y;
	double		rad;

	rad = (left_right == 0 ? 1 : -1) * (30.0 * M_PI / 180.0);

	pthread_mutex_lock(&adv->dir_mutex);
	n_x = (adv->dir.x * cos(-rad)) - (adv->dir.y) * sin(-rad);
	n_y = adv->dir.x * sin(-rad) + (adv->dir.y) * cos(-rad);
	adv->dir.y = n_y;
	adv->dir.x = n_x;
	pthread_mutex_unlock(&adv->dir_mutex);
}

static void move_random(t_enemy *adv)
{
	// printf("mov %d\nrand %d\n", adv->is_moving, adv->random_moves);
	if (adv->random_moves <= 0)
	{
		adv->random_moves = rand() % 25;
		adv->random_turn = rand() % 3;
		pthread_mutex_lock(&adv->mov_mutex);
		adv->is_moving = rand() % 2;
		pthread_mutex_unlock(&adv->mov_mutex);
		if (adv->random_turn != 2)
			turn_face(adv, adv->random_turn);
	}
		// printf("turn %d %d\n", turn, adv->random_moves);
	if (adv->is_moving)
		move_forward(adv);
	adv->random_moves--;
}

static void enemy_move(t_enemy *adv)
{
	adv->play_seen = see_player(adv);
	if (!adv->play_seen)
		return move_random(adv);
	else
		return move_random(adv);
	look_to_player(adv);
	move_forward(adv);
}

void *enemy_thread(void *data)
{
	int stop, can_move;
	t_enemy *adv;
	t_cube	*cube;

	adv = (t_enemy *)data;
	cube = adv->cube;
	pthread_mutex_lock(&cube->stop_mutex);
	stop = cube->stop;
	pthread_mutex_unlock(&cube->stop_mutex);
	pthread_mutex_lock(&cube->pause_mutex);
	can_move = !cube->pause;
	pthread_mutex_unlock(&cube->pause_mutex);
	while (!stop)
	{
		// printf("pos %f %f - %f %f", adv->pos.x, adv->pos.y, cube->player->pos.x, cube->player->pos.y);
		if (can_move)
		{
			enemy_move(adv);
		}
		// printf("Stop: %d\n", cube->stop);
		usleep(1000000 / 60);
		pthread_mutex_lock(&cube->stop_mutex);
		stop = cube->stop;
		pthread_mutex_unlock(&cube->stop_mutex);
		pthread_mutex_lock(&cube->pause_mutex);
		can_move = !cube->pause;
		pthread_mutex_unlock(&cube->pause_mutex);
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
