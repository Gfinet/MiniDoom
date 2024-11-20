/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 00:07:51 by gfinet            #+#    #+#             */
/*   Updated: 2024/11/20 00:47:32 by Gfinet           ###   ########.fr       */
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

void free_enemy(t_cube *cube, t_enemy *adv)
{
	int		i = -1;

	while (++i < adv->max_text_fr)
		mlx_destroy_image(cube->mlx, adv->spr_fr[i].img);
	i = -1;
	while (++i < adv->max_text_bk)
		mlx_destroy_image(cube->mlx, adv->spr_bk[i].img);
	i = -1;
	while (++i < adv->max_text_sd)
		mlx_destroy_image(cube->mlx, adv->spr_sd[i].img);
	mlx_destroy_image(cube->mlx, adv->text_on.img);
	free(adv->spr_fr);
	free(adv->spr_bk);
	free(adv->spr_sd);
	free(adv->path);
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
		if (cube->lvl->nb_enemy > 0)
		{
			free(cube->hit_data.enemies_dist);
			free(cube->hit_data.enemies_hit);
		}
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
	if (cube->lvl->enemy)
	{
		while (++i < cube->lvl->nb_enemy)
			free_enemy(cube, &cube->lvl->enemy[i]);
		free(cube->lvl->enemy);
		printf("enemies freed\n");
	}
		
}

int out_of_maps(t_maps *maps, int x, int y)
{
	return (x > maps->max_len || y > maps->m_height);
}
