/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 00:07:51 by gfinet            #+#    #+#             */
/*   Updated: 2024/10/31 15:41:56 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	free_and_gnl(char **str, int fd)
{
	free(*str);
	*str = get_next_line(fd);
}

int	new_img(t_cube *cube, t_data *new_img, int width, int height)
{
	new_img->img = mlx_new_image(cube->mlx, width, height);
	if (!new_img->img)
		return (0);
	new_img->addr = mlx_get_data_addr(new_img->img, &new_img->bits_per_pixel,
			&new_img->line_length, &new_img->endian);
	return (1);
}

void	free_maps(char **maps, int ind)
{
	int	i;

	i = -1;
	while (++i <= ind)
		free(maps[i]);
	free(maps);
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
	}
	cur = cube->doors;
	while (cur)
	{
		next = cur->next;
		free(cur);
		cur = next;
	}
	free_text(cube);
	if (cube->lvl->weap)
		draw_weapons(cube, 1);
}

int out_of_maps(t_maps *maps, int x, int y)
{
	return (x > maps->max_len || y > maps->m_height);
}