/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_event2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 16:36:22 by gfinet            #+#    #+#             */
/*   Updated: 2024/09/12 11:50:28 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

int	mouse_press_event(int butt, int x, int y, t_cube *cube)
{
	(void)y;
	(void)x;
	if (cube->pause)
		return (1);
	if (butt == MS_DW && cube->player->use_weap)
		cube->player->use_weap--;
	else if (butt == MS_DW && !cube->player->use_weap)
		cube->player->use_weap = cube->lvl->nb_weap - 1;
	if (butt == MS_UP && cube->player->use_weap < cube->lvl->nb_weap - 1)
		cube->player->use_weap++;
	else if (butt == MS_UP && cube->player->use_weap == cube->lvl->nb_weap - 1)
		cube->player->use_weap = 0;
	if (butt == MS_L_CLK)
		cube->player->shoot = 1;
	if (butt == MS_SCR_CLK)
		cube->s_mouse = !cube->s_mouse;
	return (1);
}

int	mouse_rel_event(int keycode, int x, int y, t_cube *cube)
{
	(void)y;
	(void)x;
	if (keycode == MS_L_CLK)
		cube->player->shoot = 0;
	return (1);
}

void	set_use_weapon(int key, t_cube *cube)
{
	int	*val;
	int	i;

	val = (int [11]){18, 19, 20, 21, 23, 22, 26, 28, 25, 29, 0};
	i = 0;
	while (val[i] && val[i] != key)
		i++;
	if (i < cube->lvl->nb_weap && val[i])
		cube->player->use_weap = i;
}
