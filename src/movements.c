/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 16:08:53 by lvodak            #+#    #+#             */
/*   Updated: 2024/09/12 14:45:47 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void	update_player(t_cube *cb, t_player *play)
{
	t_point	n_pos;
	double	n_x;
	double	n_y;
	double	rad;

	rad = (90) * (M_PI / 180.0);
	cb->player->prev_pos = (t_point){cb->player->pos.x, cb->player->pos.y};
	n_x = (cb->player->dir.x * cos(-rad)) - (cb->player->dir.y) * sin(-rad);
	n_y = cb->player->dir.x * sin(-rad) + (cb->player->dir.y) * cos(-rad);
	n_pos.x = play->pos.x + play->move_v * (play->dir.x / (4 * cb->frame));
	n_pos.y = play->pos.y + play->move_v * (play->dir.y / (4 * cb->frame));
	n_pos.x += play->move_h * (n_x / (4 * cb->frame));
	n_pos.y += play->move_h * (n_y / (4 * cb->frame));
	if (play->turn)
		turn(cb, 11.25 * play->turn, cb->frame);
	if (!impassable(cb->lvl->c_maps, cb, n_pos.x, play->pos.y))
		play->pos.x = n_pos.x;
	if (!impassable(cb->lvl->c_maps, cb, play->pos.x, n_pos.y))
		play->pos.y = n_pos.y;
}

void	set_angle(t_cube *cube, int x, int y)
{
	t_player	*play;

	play = cube->player;
	(void)y;
	(void)x;
	if (play->turn == 1 || play->turn == -1 || cube->s_mouse)
		play->angle = 45 * play->turn;
	else if (play->turn == 2 || play->turn == -2)
	{
		if ((x > 0 && x <= 600) || (x > 600 && x < 1200))
			play->angle = 90 * (600 - x) / (1.5 * 360);
	}
	else
		play->angle = 45 * play->turn;
}

void	turn(t_cube *cube, double angle, int frame)
{
	double		n_x;
	double		n_y;
	double		rad;
	t_player	*play;

	(void)frame;
	play = cube->player;
	angle = play->angle;
	if (play->turn == 1 || play->turn == -1)
		rad = (angle) / (M_PI * 180.0);
	else if (cube->s_mouse)
		rad = (angle) / (2 * M_PI * 180.0);
	else
		rad = (20 * angle / cube->m_sensi) / (M_PI * 180.0);
	n_x = (cube->player->dir.x * cos(-rad)) - (cube->player->dir.y) * sin(-rad);
	n_y = cube->player->dir.x * sin(-rad) + (cube->player->dir.y) * cos(-rad);
	cube->player->dir.y = n_y;
	cube->player->dir.x = n_x;
}
