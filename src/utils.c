/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 00:07:51 by gfinet            #+#    #+#             */
/*   Updated: 2024/11/10 01:38:22 by Gfinet           ###   ########.fr       */
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

void free_enemy(t_cube *cube)
{
	int		i = -1;
	t_enemy *adv;

	adv = cube->lvl->enemy;
	while (++i < adv[0].max_text_fr)
		mlx_destroy_image(cube->mlx, adv[0].spr_fr[i].img);
	i = -1;
	while (++i < adv[0].max_text_bk)
		mlx_destroy_image(cube->mlx, adv[0].spr_bk[i].img);
	i = -1;
	while (++i < adv[0].max_text_sd)
		mlx_destroy_image(cube->mlx, adv[0].spr_sd[i].img);
	free(adv[0].spr_fr);
	free(adv[0].spr_bk);
	free(adv[0].spr_sd);
	free(adv[0].path);
}

void	free_cube(t_cube *cube)
{
	t_door	*cur;
	t_door	*next;

	if (cube->lvl)
	{
		free_maps(cube->lvl->c_maps, cube->lvl->m_height - 1);
		if (cube->lvl->c_text)
			free_maps(cube->lvl->c_text, 3);
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
		free_enemy(cube);
		free(cube->lvl->enemy);
		printf("enemies freed\n");
	}
		
}

int out_of_maps(t_maps *maps, int x, int y)
{
	return (x > maps->max_len || y > maps->m_height);
}
