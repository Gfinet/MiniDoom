/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_weapon.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 00:26:20 by Gfinet            #+#    #+#             */
/*   Updated: 2024/11/10 01:45:14 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void	put_weapon(t_cube *cube, int *i)
{
	int			w;
	int			h;
	int			u_w;
	t_weapon	*weap;

	u_w = cube->player->use_weap;
	weap = cube->lvl->weap;
	if (cube->player->shoot == 1)
	{
		w = (WIN_WIDTH - weap[u_w].sprites[i[u_w]].width) / 2;
		h = WIN_HEIGHT - weap[u_w].sprites[i[u_w]].height;
		mlx_put_image_to_window(cube->mlx, cube->win,
			weap[u_w].sprites[i[u_w]].img, w, h);
	}
	else
	{
		w = (WIN_WIDTH - weap[u_w].sprites[0].width) / 2;
		h = WIN_HEIGHT - weap[u_w].sprites[0].height;
		mlx_put_image_to_window(cube->mlx, cube->win,
			weap[u_w].sprites[0].img, w, h);
	}
}

void	draw_weapons(t_cube *cube)
{
	int			n;
	int			u_w;
	static int	i[3] = {0, 0, 0};
	static int	fps = 0;
	t_weapon	*weap;

	weap = cube->lvl->weap;
	u_w = cube->player->use_weap;
	n = 0;
	while (weap[u_w].path[n] != 0)
		n++;
	put_weapon(cube, i);
	fps++;
	if (fps - 1 == (cube->frame / (1 + cube->player->run)) / 4)
		i[u_w]++;
	i[u_w] %= (n - (u_w != 0));
	fps %= cube->frame + cube->frame * cube->player->run;
}
