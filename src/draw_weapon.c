/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_weapon.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 00:26:20 by Gfinet            #+#    #+#             */
/*   Updated: 2026/09/08 13:46:50 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void	put_weapon(t_cube *cube)
{
	int			w;
	int			h;
	int			u_w;
	int			*spr;
	t_weapon	*weap;

	u_w = cube->player->use_weap;
	weap = cube->lvl->weap;
	spr = &weap[u_w].use_spr;
	if (cube->player->shoot == 1 || *spr != 1)
	{
		w = (WIN_WIDTH - weap[u_w].sprites[*spr].width) / 2;
		h = WIN_HEIGHT - weap[u_w].sprites[*spr].height;
		mlx_put_image_to_window(cube->mlx, cube->win,
			weap[u_w].sprites[*spr].img, w, h);
	}
	else
	{
		w = (WIN_WIDTH - weap[u_w].sprites[0].width) / 2;
		h = WIN_HEIGHT - weap[u_w].sprites[0].height;
		mlx_put_image_to_window(cube->mlx, cube->win,
			weap[u_w].sprites[0].img, w, h);
		*spr = 0;
	}
}

void	draw_weapons(t_cube *cube)
{
	int			len;
	int			u_w;
	int			*spr;
	static int	fps = 0;
	t_weapon	*weap;

	weap = cube->lvl->weap;
	u_w = cube->player->use_weap;
	spr = &weap[u_w].use_spr;
	len = weap[u_w].pathLen;
	
	put_weapon(cube);
	fps++;
	if (fps - 1 == (cube->frame / (1 + cube->player->run))) //speed  btw sprites
		(*spr)++;
	*spr %= (len - 1); //n - 1 avoid the img
	if (!*spr) (*spr)++; //avoid the standing sprite

	if (fps >= 2 * cube->frame + cube->frame * cube->player->run)
		fps %= cube->frame; 
	
}
