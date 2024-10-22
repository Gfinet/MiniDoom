/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_weapon.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 00:26:20 by Gfinet            #+#    #+#             */
/*   Updated: 2024/09/12 11:50:28 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

static void	set_i(int **i, int nb)
{
	int	x;

	x = -1;
	*i = malloc(nb * sizeof(int));
	while (++x < nb)
		(*i)[x] = 0;
}

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

void	draw_weapons(t_cube *cube, int fre)
{
	int			n;
	int			u_w;
	static int	*i = 0;
	static int	fps = 0;
	t_weapon	*weap;

	if (i == 0)
		set_i(&i, cube->lvl->nb_weap);
	weap = cube->lvl->weap;
	u_w = cube->player->use_weap;
	n = 0;
	while (weap[u_w].path[n] != 0)
		n++;
	if (!fre)
		put_weapon(cube, i);
	fps++;
	if (fps - 1 == (cube->frame / (1 + cube->player->run)) / 4)
		i[u_w]++;
	i[u_w] %= (n - (u_w != 0));
	fps %= cube->frame + cube->frame * cube->player->run;
	if (fre)
		free(i);
}

int	xpm_to_img(t_cube *cube, t_data *new_img, char *name)
{
	new_img->img = mlx_xpm_file_to_image(cube->mlx, name,
			&new_img->width, &new_img->height);
	if (!new_img->img)
		return (0);
	new_img->addr = mlx_get_data_addr(new_img->img, &new_img->bits_per_pixel,
			&new_img->line_length, &new_img->endian);
	return (1);
}
