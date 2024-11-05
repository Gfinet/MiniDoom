/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_enemy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:02:38 by gfinet            #+#    #+#             */
/*   Updated: 2024/11/05 22:07:18 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

// void enemy_routine(t_cube *cube)
// {

// }

int enemy_in_sight(t_cube *cube, t_rcdata *data)
{
	int		i;
	t_enemy	*adv;
	t_point	posi;
	t_point hit;

	adv = cube->lvl->enemy;
	i = 0;
	while (i < cube->lvl->nb_enemy)
	{
		posi = adv[i].pos;
		hit = adv[i].hitbox;
		if (posi.x - hit.x / 2 > (int)data->dest.x && posi.x + hit.x / 2 < (int)data->dest.x + 1 \
		&& posi.y - hit.y / 2 > (int)data->dest.y && posi.y + hit.y / 2 < (int)data->dest.x + 1)
			return (adv[i].id);
		i++;
	}
	return (0);
}

void set_enemy_pos(t_maps *lvl, t_enemy *adv, int nb)
{
	int 	adv_nb;
	int		i;
	int		j;

	i = 0;
	adv_nb = 0;
	while (lvl->c_maps && lvl->c_maps[i] && i < lvl->m_height)
	{
		j = 0;
		while (lvl->c_maps[i][j] && j < lvl->max_len - 1)
		{
			if (lvl->c_maps[i][j] == 'A')
				adv_nb++;
			if (adv_nb == nb)
				break;
			j++;
		}
		if (adv_nb == nb)
				break;
		i++;
	}
	adv->pos.x = (double)j + 0.5;
	adv->pos.y = (double)i + 0.5;
	adv->hitbox.x = 0.4;
	adv->hitbox.y = 1;
	adv->id = nb;
}

void set_enemy(t_maps *lvl, char *str)
{
	int		len = 1;
	char	*tmp;
	char	**lst;


	if (!lvl->enemy)
	{
		lvl->enemy = malloc(sizeof(t_enemy));
		if (!lvl->enemy)
			return ;
	}
	lst = ft_split(str, ' ');
	tmp = ft_substr(lst[len - 1], 0, ft_strlen(lst[len - 1]) - 1);
	free(lst[len - 1]);
	lst[len - 1] = tmp;
	lst[len] = 0;
	lvl->enemy[0].path = lst;
}

int get_enemy_inf(t_cube *cube)
{
	t_enemy *adv;
	
	adv = cube->lvl->enemy;

	adv[0].spr_fr = malloc(sizeof(t_data));
	if (!adv[0].spr_fr)
		return (printf("enemy sprites malloc error\n"), 0);
	adv[0].spr_fr[0].width = 80;
	adv[0].spr_fr[0].height = 120;
	xpm_to_img(cube, &adv[0].spr_fr[0], adv[0].path[0]);
	if (!adv[0].spr_fr[0].img)
		return (printf("enemy sprites loading error\n"), 0);
	adv->dmg = 5;
	set_enemy_pos(cube->lvl, &cube->lvl->enemy[0], 1);
	return (1);
}

int check_enemy_inf(t_cube *cube, char *str)
{
	//int		i;
	int		len;
	char	*tmp;
	char	**lst;
	t_data	data;

	len = 0;
	lst = ft_split(&str[1], ' ');
	while (lst[len] != 0)
		len++;
	tmp = ft_substr(lst[len - 1], 0, ft_strlen(lst[len - 1]) - 1);
	free(lst[len - 1]);
	lst[len - 1] = tmp;
	data.width = 80;
	data.height = 120;
	data.img = mlx_xpm_file_to_image(cube->mlx, lst[0], &data.width, &data.height);
	printf("%s\n", lst[0]);
	if (!data.img)
		return (printf("Enemy texture error\n"), 0);
	return (free_maps(lst, len), cube->lvl->nb_enemy++, 1);
}