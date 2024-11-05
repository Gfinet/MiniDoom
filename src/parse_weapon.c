/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_weapon.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 21:10:01 by Gfinet            #+#    #+#             */
/*   Updated: 2024/11/03 18:15:33 by Gfinet           ###   ########.fr       */
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
		weap[i].sprites = malloc(sizeof(t_data) * len);
		if (!weap[i].sprites)
			return (printf("gun sprites malloc error\n"), 0);
		weap[i].sprites[0].width = 10;
		weap[i].sprites[0].height = 17;
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

void	set_weapon(t_maps *lvl, char *str)
{
	static int	i = 0;
	char		*tmp;
	char		**lst;
	size_t		len;

	if (!lvl->weap)
	{
		lvl->weap = malloc(lvl->nb_weap * sizeof(t_weapon));
		if (!lvl->weap)
			return ;
	}
	lst = ft_split(str, ' ');
	len = 0;
	while (lst[len] != 0)
		len++;
	tmp = ft_substr(lst[len - 1], 0, ft_strlen(lst[len - 1]) - 1);
	free(lst[len - 1]);
	lst[len - 1] = tmp;
	lst[len] = 0;
	lvl->weap[i].path = lst;
	i++;
}

int	check_weapon(t_cube *cube, char *str)
{
	int		i;
	int		len;
	char	*tmp;
	char	**lst;
	t_data	data;

	len = 0;
	lst = ft_split(&str[1], ' ');
	while (lst[len] != 0)
		len++;
	if (len == 1)
		return (free_maps(lst, len), printf("gun %s lack sprite\n", lst[0]), 0);
	tmp = ft_substr(lst[len - 1], 0, ft_strlen(lst[len - 1]) - 1);
	free(lst[len - 1]);
	lst[len - 1] = tmp;
	i = -1;
	while (++i < len)
	{
		data.width = 100;
		data.height = 170;
		data.img = mlx_xpm_file_to_image(cube->mlx, lst[i],
				&data.width, &data.height);
		if (!data.img)
			return (printf("Gun texture error\n"), 0);
	}
	return (free_maps(lst, len), cube->lvl->nb_weap++, 1);
}
