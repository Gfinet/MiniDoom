/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interaction.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 16:12:12 by lvodak            #+#    #+#             */
/*   Updated: 2024/09/12 12:25:39 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void	find_and_open_door(t_door *door, t_player *pl)
{
	t_point	pos;

	while (door)
	{
		pos = door->coord;
		if (((int)pos.x <= (int)fmax(pl->pos.x + pl->dir.x, pl->pos.x)
				&& (int)pos.x >= (int)fmin(pl->pos.x + pl->dir.x, pl->pos.x))
			&& ((int)pos.y <= (int)fmax(pl->pos.y + pl->dir.y, pl->pos.y)
				&& (int)pos.y >= (int)fmin(pl->pos.y + pl->dir.y, pl->pos.y))
			&& !((int)pl->pos.x == (int)pos.x && (int)pl->pos.y == (int)pos.y))
		{
			if (door->open == 0)
			{
				door->open = 1;
				door->on_going = 1;
			}
			else if (door->open == 30)
			{
				door->open = 29;
				door->on_going = -1;
			}
			return ;
		}
		door = door->next;
	}
}

int	impassable(char **map, t_cube *cb, int x, int y)
{
	//t_door	*door;

	(void)cb;
	if (map[y][x] == '1' || map[y][x] == '2')
		return (1);
	// if (map[y][x] == 'D')
	// {
	// 	door = find_door(cb, x, y);
	// 	if (door->open != 30)
	// 		return (1);
	// }
	return (0);
}
