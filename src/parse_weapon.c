/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_weapon.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:10:01 by Gfinet            #+#    #+#             */
/*   Updated: 2026/09/24 01:20:46 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

int	get_weapon(t_cube *cube)
{
	int			i;
	int			j;
	size_t		len;
	t_weapon	*weap;

	weap = cube->lvl->weap;
	i = -1;
	while (++i < cube->lvl->nb_weap)
	{
		len = 0;
		while (weap[i].path[len])
			len++;
		weap[i].pathLen = len;
		weap[i].sprites = calloc(sizeof(t_data), len);
		if (!weap[i].sprites)
			return (printf("gun sprites malloc error\n"), 0);
		weap[i].use_spr = 0;
		j = -1;
		while (weap[i].path[++j])
		{
			xpm_to_img(cube, &weap[i].sprites[j], weap[i].path[j]);
			if (!weap[i].sprites[j].img)
				return (printf("gun sprites loading error\n"), 0);
		}
		weap->dmg = 50;
	}
	return (1);
}

void	set_weapon(t_lvl *lvl, char *str)
{
	static int	i = 0;
	char		*tmp;
	char		**lst;
	size_t		len;

	if (!lvl->weap)
	{
		lvl->weap = calloc(lvl->nb_weap, sizeof(t_weapon));
		if (!lvl->weap)
			return ;
	}//G Name dmg speed

	lst = ft_split(str, ' ');
	ft_bzero(lvl->weap[i].name, 10);
	ft_strlcat(lvl->weap[i].name, lst[0], ft_strlen(lst[0]) + 1);
	printf("name %s\n", lvl->weap[i].name);
	len = 0;
	while (lst[len] != 0)
		len++;
	tmp = ft_substr(lst[len - 1], 0, ft_strlen(lst[len - 1]) - 1);
	free(lst[len - 1]);
	lst[len - 1] = tmp;
	lst[len] = 0;
	// lvl->weap[i].name = ft_atoi(lst[0]);
	lvl->weap[i].dmg = ft_atoi(lst[1]);
	lvl->weap[i].freq_atk = ft_atoi(lst[2]);
	i++;
}

static int get_weapon_spr(t_cube *cube, t_data **text, char *path, char *name, int ind)
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
	//cube->lvl->weap[ind].sprite[ind]
	xpm_to_img(cube, &(*text)[ind], file);
	printf("File %s loaded\n", file);
	free(file);
	if (!(*text)[ind].img)
		return (printf("Enemy sprites loading error\n"), 0);
	
	return 1;
}

int get_weapon_inf(t_cube *cube, int ind)
{
	char			*name;
	t_weapon 		*weap = 0;
	t_data			**text;
	size_t 			len = 0, nb = 0;
	char			*dir_path;
	struct dirent	**dir;
	
	weap = &cube->lvl->weap[ind];
	name = weap->name;
	dir_path = ft_strjoin("./weapon_sprites/", name);
	len = scandir(dir_path, &dir, NULL, alphasort);
	if (!dir || len <= 0)
        return (printf("Error while opening %s\n", dir_path), 0);
	// printf("len %zu %s\n", len, name);
	weap->pathLen = len - 2;
	text = &weap->sprites;
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
				if (!get_weapon_spr(cube, text, dir_path, dir[i]->d_name, nb))
					return free(dir), free(dir_path), 0;
				nb++;
			}
			free(dir[i]);
		}
	}
	free(dir);
	free(dir_path);
	return 1;
}

int	check_weapon(t_cube *cube, char *str)
{
	int		len;
	char	*tmp;
	char	**lst;

	len = 0;
	lst = ft_split(&str[1], ' ');
	while (lst[len] != 0)
		len++;
	if (len < 3)
		return (free_maps(lst, len), printf("gun %s lack sprite\n", lst[0]), 0);
	tmp = ft_substr(lst[len - 1], 0, ft_strlen(lst[len - 1]) - 1);
	free(lst[len - 1]);
	lst[len - 1] = tmp;
	return (free_maps(lst, len), cube->lvl->nb_weap++, 1);
}
