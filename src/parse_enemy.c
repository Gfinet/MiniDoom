/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_enemy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:02:38 by gfinet            #+#    #+#             */
/*   Updated: 2024/11/10 17:57:22 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void set_draw_enemy(t_cube *cube, int val)
{
	int		i = -1;

	while (++i < cube->lvl->nb_enemy)
		cube->lvl->enemy[i].draw = val;
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
	adv->pos = (t_point){j + 0.5, i + 0.5};
	adv->dir = (t_point){0, -1};
	adv->hitbox = (t_point){0.4, 1};
	adv->id = nb;
}

void set_enemy(t_maps *lvl, char *str)
{
	int		len = 0;
	char	*tmp;
	char	**lst;


	if (!lvl->enemy)
	{
		lvl->enemy = malloc(sizeof(t_enemy));
		if (!lvl->enemy)
			return ;
		*lvl->enemy = (t_enemy){0};
	}
	lst = ft_split(str, ' ');
	while (lst[len])
		len++;
	tmp = ft_substr(lst[len - 1], 0, ft_strlen(lst[len - 1]) - 1);
	free(lst[len - 1]);
	lst[len - 1] = tmp;
	lst[len] = 0;
	lvl->enemy[0].path_len = len;
	lvl->enemy[0].path = lst;
	lvl->enemy[0].dir = (t_point){1,1};
}

static t_data **get_ptr_texture(t_enemy *adv, int *num)
{
	t_data **text = NULL;

	if (*num == 0)
		text = &adv->spr_fr;
	else if (*num == 1)
		text = &adv->spr_bk;
	else if (*num == 2)
		text = &adv->spr_sd;
	return ((*num)++, text);
}

static int *get_ptr_len(t_enemy *adv, int *num)
{
	int 	*len = 0;
	
	if (*num == 0)
		len = &adv->max_text_fr;
	else if (*num == 1)
		len = &adv->max_text_bk;
	else if (*num == 2)
		len = &adv->max_text_sd;
	return (len);
}

int get_enemy_inf(t_cube *cube)
{
	int		first_text, i, j, l;
	int		*len;
	t_enemy *adv;
	t_data	**text = NULL;
	
	adv = cube->lvl->enemy;
	i = -1;
	l = 0;
	while (++i < adv[0].path_len - 1)
	{
		first_text = i;
		while (i < adv[0].path_len - 1 && !ft_strncmp(adv[0].path[i], adv[0].path[i + 1], ft_strlen(adv[0].path[i]) - 5))
			i++;
		len = get_ptr_len(&adv[0], &l);
		*len = i - first_text + 1;
		text = get_ptr_texture(&adv[0], &l);
		if (!*text)
			*text = malloc(sizeof(t_data) * ((*len) + 1));
		if (!*text)
			return (0);
		j = -1;
		while (++j < *len)
		{
			(*text)[j].width = 100;
			(*text)[j].height = 150;
			xpm_to_img(cube, &(*text)[j], adv[0].path[first_text + j]);
			if (!(*text)[j].img)
				return (printf("enemy sprites loading error\n"), 0);
		}
	}
	set_enemy_pos(cube->lvl, &cube->lvl->enemy[0], 1);
	return (1);
}

int check_enemy_inf(t_cube *cube, char *str)
{
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
	if (!data.img)
		return (printf("Enemy texture error\n"), 0);
	return (free_maps(lst, len), cube->lvl->nb_enemy++, 1);
}
