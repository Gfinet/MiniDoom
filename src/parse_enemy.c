/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_enemy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:02:38 by gfinet            #+#    #+#             */
/*   Updated: 2026/09/03 04:50:35 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void set_draw_enemy(t_cube *cube, int val)
{
	int		i = -1;

	while (++i < cube->lvl->nb_enemy)
	{
		cube->lvl->enemies[i].draw = val;
		cube->lvl->enemies[i].short_dist = 999;
		cube->lvl->enemies[i].tmp_dist = 0;
		cube->lvl->enemies[i].ray_hit = 0;
		cube->lvl->enemies[i].ray_max = 0;
		cube->lvl->enemies[i].l_r = 0;
	}
	
}

int set_enemy_pos(t_maps *lvl)
{
	t_enemy 	*adv;
	int 		adv_nb;
	int			i;
	int			j;
	int 		ind;

	i = 0;
	adv_nb = 0;
	while (lvl->c_maps && lvl->c_maps[i] && i < lvl->m_height)
	{
		j = 0;
		while (lvl->c_maps[i][j] && j < lvl->max_len - 1)
		{
			if (lvl->c_maps[i][j] == 'A')
				adv_nb++;
			j++;
		}
		i++;
	}
	if (!lvl->enemies)
	{
		lvl->enemies = malloc(sizeof(t_enemy) * adv_nb);
		if (!lvl->enemies)
			return 0;
	}
	lvl->nb_enemy = adv_nb;
	ind = 0;
	i = 0;
	adv = &lvl->enemies[ind];
	while (lvl->c_maps && lvl->c_maps[i] && i < lvl->m_height)
	{
		j = 0;
		while (lvl->c_maps[i][j] && j < lvl->max_len - 1)
		{
			if (lvl->c_maps[i][j] == 'A')
			{
				adv->type = &lvl->enemy_types[0];
				adv->pos = (t_point){j + 0.5, i + 0.5};
				adv->dir = (t_point){0, -1};
				adv->hitbox = (t_point){0.8, 0.8};
				adv->id = ind;
				ind++;
				adv = &lvl->enemies[ind];
				printf("Got %d en %d %d\n", ind, i, j);
			}
			if (ind == adv_nb)
				break;
			// printf("%d %d ", i, j);
			j++;
		}
		if (ind == adv_nb)
			break;
		i++;
	}
	return 1;
}

void set_enemy(t_maps *lvl, char *str)
{
	int		len = 0, i = -1;
	char	*tmp;
	char	**lst;

	lvl->nb_enemy_type++;
	if (!lvl->enemy_types)
	{
		lvl->enemy_types = malloc(sizeof(t_enemy_type) * lvl->nb_enemy_type);
		if (!lvl->enemy_types)
			return ;
		*lvl->enemy_types = (t_enemy_type){0};
	}
	lst = ft_split(str, ' ');
	while (lst[len])
		len++;
	tmp = ft_substr(lst[len - 1], 0, ft_strlen(lst[len - 1]) - 1);
	free(lst[len - 1]);
	lst[len - 1] = tmp;
	lst[len] = 0;
	while (++i < lvl->nb_enemy_type)
	{
		lvl->enemy_types[i].path_len = len;
		lvl->enemy_types[i].path = lst;
	}
}

static t_data **get_ptr_texture(t_enemy_type *adv, int *num)
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

static int *get_ptr_len(t_enemy_type *adv, int *num)
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

void set_first_hitb(t_cube *cube, t_enemy *adv)
{
	//t_img_mlx	*img;
	t_data		spr;
	//double		dist;

	// xpm_to_img(cube, &spr, adv->path[0]);
	//img = spr.img;
	//dist = dist_ab(cube->player->pos, adv->pos);
	// if (dist == 0)
		adv->bobox = 1;
	// else
	// 	adv->hitb = (img->width * 6) / (dist * WIN_WIDTH);
	mlx_destroy_image(cube->mlx, spr.img);
}

int get_enemy_inf(t_cube *cube, int ind)
{
	int				first_text, i, j, l;
	int				*len;
	t_enemy_type 	*adv;
	t_data			**text = NULL;
	
	adv = cube->lvl->enemy_types;
	i = -1;
	l = 0;
	while (++i < adv[ind].path_len - 1)
	{
		first_text = i;
		while (i < adv[ind].path_len - 1 && !ft_strncmp(adv[ind].path[i], adv[ind].path[i + 1], ft_strlen(adv[ind].path[i]) - 5))
			i++;
		len = get_ptr_len(&adv[ind], &l);
		*len = i - first_text + 1;
		text = get_ptr_texture(&adv[ind], &l);
		if (!*text)
			*text = malloc(sizeof(t_data) * ((*len) + 1));
		if (!*text)
			return (0);
		j = -1;
		while (++j < *len)
		{
			(*text)[j].width = 100;
			(*text)[j].height = 150;
			xpm_to_img(cube, &(*text)[j], adv[ind].path[first_text + j]);
			if (!(*text)[j].img)
				return (printf("enemy sprites loading error\n"), 0);
		}
	}
	set_enemy_pos(cube->lvl);
	// set_first_hitb(cube, &adv[ind]);
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
	return (free_maps(lst, len), 1);
}
