/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_event.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 21:04:25 by gfinet            #+#    #+#             */
/*   Updated: 2024/09/12 12:17:53 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

int	esc_handle(t_cube *cube)
{
	if (cube->win)
	{
		mlx_clear_window(cube->mlx, cube->win);
		mlx_destroy_window(cube->mlx, cube->win);
	}
	free_cube(cube);
	exit(0);
	return (0);
}

int	fps(t_cube	*cube)
{
	int			i;
	t_player	*p;
	//t_door		*d;

	i = 0;
	p = cube->player;
	if (cube->pause)
		pause_screen(cube);
	//d = find_door(cube, p->pos.x + p->dir.x, p->pos.y + p->dir.y);
	// if (d && d->on_going)
	// 	i = -cube->frame;
	while (i++ < cube->frame && !cube->pause)
	{
		update_player(cube, p);
		draw_doom(cube);
		mlx_do_sync(cube->mlx);
	}
	if (cube->mouse)
	{
		cube->mouse = 0;
		cube->player->turn = 0;
	}
	return (1);
}

int	key_event(int keycode, t_cube *cube)
{
	if (cube->pause)
		return (choose_pause(keycode, cube));
	if (keycode == ESC)
		cube->pause = !cube->pause;
	if (keycode == W)
		cube->player->move_v = 1;
	if (keycode == S)
		cube->player->move_v = -1;
	if (keycode == D)
		cube->player->move_h = -1;
	if (keycode == A)
		cube->player->move_h = 1;
	if (keycode == RIGHT || keycode == E)
		cube->player->turn = -1;
	if (keycode == LEFT || keycode == Q)
		cube->player->turn = 1;
	if (keycode == L_SH)
	{
		cube->player->run = 1;
		cube->frame = FRAME / 2;
	}
	set_angle(cube, 0, 0);
	if (keycode > 17 && keycode < 29 && keycode != 24 && keycode != 27)
		set_use_weapon(keycode, cube);
	return (1);
}

int	key_event_release(int keycode, t_cube *cube)
{
	if (keycode == W || keycode == S)
		cube->player->move_v = 0;
	if (keycode == D || keycode == A)
		cube->player->move_h = 0;
	if (keycode == Q || keycode == E || keycode == LEFT || keycode == RIGHT)
		cube->player->turn = 0;
	if (keycode == L_SH)
	{
		cube->player->run = 0;
		cube->frame = FRAME;
	}
	if (keycode == P && cube->m_sensi > 1)
		cube->m_sensi--;
	if (keycode == O && cube->m_sensi < 10)
		cube->m_sensi++;
	// if (keycode == F)
	// 	find_and_open_door(cube->doors, cube->player);
	return (1);
}

int	mouse_event(int x, int y, t_cube *cube)
{
	(void)y;
	if (cube->pause)
		return (1);
	mlx_mouse_hide();
	cube->mouse = 1;
	if (x >= WIN_WIDTH * 0.5 && x <= WIN_WIDTH * 0.5)
		cube->player->turn = 0;
	if (x < WIN_WIDTH * 0.5)
		cube->player->turn = 2;
	if (x > WIN_WIDTH * 0.5)
		cube->player->turn = -2;
	set_angle(cube, x, y);
	mlx_mouse_move(cube->win, WIN_WIDTH / 2, WIN_HEIGHT / 2);
	return (0);
}
