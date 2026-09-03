/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 00:07:51 by gfinet            #+#    #+#             */
/*   Updated: 2026/09/03 16:09:17 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void	free_and_gnl(char **str, int fd)
{
	free(*str);
	*str = get_next_line(fd);
}

void	free_maps(char **maps, int ind)
{
	int	i;

	i = -1;
	while (++i <= ind)
		free(maps[i]);
	free(maps);
}

void free_weapons(t_cube *cube)
{
	int			i;
	int 		j;
	t_weapon	*weap;

	weap = cube->lvl->weap;
	i = -1;
	while (++i < cube->lvl->nb_weap)
	{
		j = 0;
		while (weap[i].path[j])
		{
			mlx_destroy_image(cube->mlx, weap[i].sprites[j].img);
			free(weap[i].path[j]);
			j++;
		}
		free(weap[i].path);
	}
}

void free_enemy(t_cube *cube, t_enemy_type *adv)
{
	int		i = -1;

	while (++i < adv->max_text_fr)
		if (adv->spr_fr[i].img)
			mlx_destroy_image(cube->mlx, adv->spr_fr[i].img);
	i = -1;
	while (++i < adv->max_text_bk)
		if (adv->spr_bk[i].img)
			mlx_destroy_image(cube->mlx, adv->spr_bk[i].img);
	i = -1;
	while (++i < adv->max_text_sd)
		if (adv->spr_sd[i].img)
			mlx_destroy_image(cube->mlx, adv->spr_sd[i].img);
	i = -1;
	while(++i < cube->lvl->nb_enemy)
		if (cube->lvl->enemies[i].text_on.img)
			mlx_destroy_image(cube->mlx, cube->lvl->enemies[i].text_on.img);
	if (adv->spr_fr)
	{
		free(adv->spr_fr);
		adv->spr_fr = 0;
	}
	if (adv->spr_bk)
	{
		free(adv->spr_bk);
		adv->spr_bk = 0;
	}
	if (adv->spr_sd)
	{
		free(adv->spr_sd);
		adv->spr_sd = 0;
	}
}

void	free_cube(t_cube *cube)
{
	int		i = -1;
	t_door	*cur;
	t_door	*next;

	if (cube->lvl)
	{
		free_maps(cube->lvl->c_maps, cube->lvl->m_height - 1);
		if (cube->lvl->c_text)
			free_maps(cube->lvl->c_text, 3);
		// if (cube->lvl->nb_enemy > 0)
		// {
			// free(cube->hit_data.enemies_dist);
			// free(cube->hit_data.enemies_hit);
		// }
		printf("maps freed\n");
	}
	cur = cube->doors;
	while (cur)
	{
		next = cur->next;
		free(cur);
		cur = next;
	}
	free_text(cube);
	printf("textures freed\n");
	if (cube->lvl->weap)
	{
		free_weapons(cube);
		free(cube->lvl->weap);
		printf("guns freed\n");
	}
	if (cube->lvl->enemies)
	{
		while (++i < cube->lvl->nb_enemy_type)
			free_enemy(cube, &cube->lvl->enemy_types[i]);
		for (int j = 0; j < cube->lvl->enemy_types[0].path_len; j++)
			free(cube->lvl->enemy_types[0].path[j]);
		free(cube->lvl->enemy_types[0].path);
		cube->lvl->enemy_types[0].path = 0;
		free(cube->lvl->enemies);
		printf("enemies freed\n");
	}
		
}

int out_of_maps(t_lvl *maps, int x, int y)
{
	return (x > maps->max_len || y > maps->m_height);
}
