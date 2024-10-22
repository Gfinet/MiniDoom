/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_maps.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:40:39 by gfinet            #+#    #+#             */
/*   Updated: 2024/09/12 11:50:28 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

int	in_char_lst(char c, char *lst)
{
	int		i;

	i = -1;
	while (++i < (int)ft_strlen(lst))
		if (lst[i] == c)
			return (1);
	return (0);
}

static int	check_surrounding(t_cube *cube, int x, int y, char c)
{
	size_t	len;
	char	**maps;

	maps = cube->lvl->c_maps;
	len = ft_strlen(maps[x]);
	if (x == 0 || y == 0 || x == cube->lvl->m_height - 1)
		return (0);
	if (y > 0 && (maps[x][y - 1] == c || maps[x][y - 1] == 0))
		return (0);
	if (y < (int)len && (maps[x][y + 1] == c || maps[x][y + 1] == 0))
		return (0);
	if (x > 0 && (maps[x - 1][y] == c || maps[x - 1][y] == 0))
		return (0);
	if (x < cube->lvl->m_height && (maps[x + 1][y] == c || maps[x + 1][y] == 0))
		return (0);
	return (1);
}

int	check_map(t_cube *cube)
{
	int	i;
	int	j;

	i = -1;
	if ((cube->lvl->m_height < 3 || cube->lvl->max_len < 3))
		return (0);
	while (++i < cube->lvl->m_height)
	{
		j = -1;
		while (++j < cube->lvl->max_len)
		{
			if (in_char_lst(cube->lvl->c_maps[i][j], "0NSWEG"))
				if (!check_surrounding(cube, i, j, '.'))
					return (0);
		}
	}
	printf("map ok\n");
	return (1);
}
