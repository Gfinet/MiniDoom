/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_enemy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:02:38 by gfinet            #+#    #+#             */
/*   Updated: 2026/09/23 20:07:01 by Gfinet           ###   ########.fr       */
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
	}
	
}

int set_enemy_pos(t_lvl *lvl)
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
		lvl->enemies = calloc(adv_nb, sizeof(t_enemy));
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
				adv->pos = (t_point){j + 0.5, i + 0.5, 0};
				adv->orig_pos = (t_point){j + 0.5, i + 0.5, 0};
				adv->dir = (t_point){0, -1, 0};//to North
				adv->hitbox = (t_point){0.4, 0.4, 0};
				adv->speed = 0.5;
				adv->id = ind;
				adv->cube = lvl->cube;
				pthread_mutex_init(&adv->pos_mutex, 0);
				pthread_mutex_init(&adv->mov_mutex, 0);
				pthread_mutex_init(&adv->dir_mutex, 0);
				pthread_mutex_init(&adv->stt_mutex, 0);
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

void set_enemy(t_lvl *lvl, char *str)
{
	t_enemy_type 	*tmp_en;
	int				len = 0;
	char			*tmp;
	char			**lst;

	
	if (!lvl->enemy_types)
	{
		lvl->enemy_types = calloc(lvl->nb_enemy_type + 1, sizeof(t_enemy_type));
		if (!lvl->enemy_types)
			return ;
	}
	else
	{
		tmp_en = realloc(lvl->enemy_types, (lvl->nb_enemy_type + 1) * sizeof(t_enemy_type));
		if (!tmp_en)
			return ;
		lvl->enemy_types = tmp_en;
	}
	lst = ft_split(str, ' ');
	ft_bzero(lvl->enemy_types[lvl->nb_enemy_type].name, 10);
	ft_strlcat(lvl->enemy_types[lvl->nb_enemy_type].name, lst[0], ft_strlen(lst[0]) + 1);
	while (lst[len])
		len++;
	tmp = ft_substr(lst[len - 1], 0, ft_strlen(lst[len - 1]) - 1); //take out \n
	free(lst[len - 1]);
	lst[len - 1] = tmp;
	lst[len] = 0;
	lvl->enemy_types[lvl->nb_enemy_type].count = ft_atoi(lst[1]);
	lvl->enemy_types[lvl->nb_enemy_type].freq_atk = ft_atoi(lst[2]);
	lvl->enemy_types[lvl->nb_enemy_type].max_hp = ft_atoi(lst[3]);
	lvl->enemy_types[lvl->nb_enemy_type].dmg= ft_atoi(lst[4]);
	lvl->nb_enemy_type++;
	free_maps(lst, len);
	return ;
	
	
	
}

static t_data **get_ptr_texture(t_enemy_type *adv, char *str)
{
	t_data **text = NULL;
	
	if (ft_strncmp(str, "front", 5) == 0)
		text = &adv->spr_fr;
	else if (ft_strncmp(str, "back", 4) == 0)
		text = &adv->spr_bk;
	else if (ft_strncmp(str, "side", 4) == 0)
		text = &adv->spr_sd;
	else if (ft_strncmp(str, "attack", 6) == 0)
		text = &adv->spr_at;
	else if (ft_strncmp(str, "death", 5) == 0)
		text = &adv->spr_dd;
	return (text);
}
static int *get_ptr_len(t_enemy_type *adv, char *str)
{
	int 	*len = 0;
	
	if (ft_strncmp(str, "front", 5) == 0)
		len = &adv->max_text_fr;
	else if (ft_strncmp(str, "back", 4) == 0)
		len = &adv->max_text_bk;
	else if (ft_strncmp(str, "side", 4) == 0)
		len = &adv->max_text_sd;
	else if (ft_strncmp(str, "attack", 6) == 0)
		len = &adv->max_text_at;
	else if (ft_strncmp(str, "death", 5) == 0)
		len = &adv->max_text_dd;
	return (len);
}

int load_enemy_texture(t_cube *cube, t_data **text, char *path, char *name, int ind)
{
	char			*file, *slash;
	size_t			ext;

	ext = ft_strlen(name) - 4;
	if (ft_strncmp(&(name[ext]), ".xpm", 5))
		return printf("Bad texture error\n"), 0;
	slash = ft_strjoin(path, "/");
	if (!slash)
		return 0;
	file = ft_strjoin(slash, name);
	free(slash);
	if (!file)
		return 0;
	(*text)[ind].width = 100;
	(*text)[ind].height = 150;
	xpm_to_img(cube, &(*text)[ind], file);
	printf("File %s loaded\n", file);
	free(file);
	if (!(*text)[ind].img)
		return (printf("Enemy sprites loading error\n"), 0);
	
	return 1;
}


int get_enemy_part(t_cube *cube, char *path, char *name, int ind)
{
	t_enemy_type 	*adv;
	t_data			**text;
	int				*len_text;
	size_t 			len = 0, nb = 0;
	char			*dir_path, *slash;
	struct dirent	**dir;

	adv = &cube->lvl->enemy_types[ind];

	slash = ft_strjoin(path, "/");
	if (!slash)
		return 0;
	dir_path = ft_strjoin(slash, name);
	free(slash);
	if (!dir_path)
		return 0;

	len = scandir(dir_path, &dir, NULL, alphasort);
	if (!dir || len <= 0)
        return (printf("Error while opening %s %p\n", path, dir), 0);
	len_text = get_ptr_len(adv, name);
	*len_text = len - 2;
	text = get_ptr_texture(adv, name);
	*text = calloc(len - 2, sizeof(t_data));
	if (!(*text))
		return 0;
	nb = 0;
	for (size_t i = 2; i < len; i++)
    {
		// printf("entry : %s/%s\n", dir_path, dir[i]->d_name);
		if (dir[i]->d_name[0] != '.')
        {
			if (dir[i]->d_type == DT_REG)
			{
				if (!load_enemy_texture(cube, text, dir_path, dir[i]->d_name, nb))
					return 0;
				nb++;
			}
			free(dir[i]);
		}
	}
	free(dir);
	free(dir_path);
	return 1;
}

int get_enemy_inf(t_cube *cube, int ind)
{
	char			*name, *path;
	t_enemy_type 	*adv;
	DIR				*dir;
	struct dirent	*entry;
	
	adv = &cube->lvl->enemy_types[ind];
	name = adv->name;
	path = ft_strjoin("./enemy_sprites/", name);
	dir = opendir(path);
	if (!dir)
        return (printf("Error while opening %s %p\n", path, dir), 0);
	entry = readdir(dir);
	while (entry != NULL)
	{
		// printf("entry : %s\n", entry->d_name);
		if (ft_strncmp(entry->d_name, ".", 2) == 0 || ft_strncmp(entry->d_name, "..", 3) == 0)
		{
			entry = readdir(dir);
			continue ;
		}
		
		if (entry->d_type == DT_DIR)
			if (!get_enemy_part(cube, path, entry->d_name, ind))
				return free(path), 0;
		entry = readdir(dir);
	}
	closedir(dir);
	free(path);
	if (!set_enemy_pos(cube->lvl))
		return 0;
	return 1;
}

int check_enemy_inf(t_cube *cube, char *str)
{//A Name Number
	int		len;
	char	**lst;

	(void)cube;
	len = 0;
	lst = ft_split(&str[1], ' ');
	while (lst[len] != 0)
		len++;
	if (len < 5)
		return (printf("lack Enemy data error\n[A Name Number AtkSpeed Hp dmg]\n"), 0);
	return (free_maps(lst, len), 1);
}
