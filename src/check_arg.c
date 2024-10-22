/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:40:39 by gfinet            #+#    #+#             */
/*   Updated: 2024/09/12 11:50:28 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

static int	check_texture(t_cube *cube, char *str)
{
	t_data	data;
	size_t	len;
	int		size[3];
	char	*tmp;

	size[0] = 5;
	size[1] = 5;
	size[2] = 0;
	if (str[0] == 'G')
		return (check_weapon(cube, &str[1]));
	while (str[size[2] + 2] == ' ')
		size[2]++;
	len = ft_strlen(&str[2]) - (size[2] + 1);
	tmp = ft_substr(&str[2], size[2], len);
	data.img = mlx_xpm_file_to_image(cube->mlx,
			tmp, &size[0], &size[1]);
	free(tmp);
	if (!data.img)
		return (0);
	return (1);
}

static int	check_all_text(t_cube *cube, char *file)
{
	int		fd;
	int		dir[5];
	char	*str;

	fd = open(file, O_RDONLY);
	ft_memset(dir, 0, sizeof(int) * 4);
	str = get_next_line(fd);
	while (str)
	{
		if (str && ((!ft_strncmp(str, "NO", 2))
				|| (!ft_strncmp(str, "SO", 2))
				|| (!ft_strncmp(str, "WE", 2))
				|| (!ft_strncmp(str, "G ", 2))
				|| (!ft_strncmp(str, "EA", 2))))
		{
			if (!check_texture(cube, str))
				return (close(fd), 0);
			dir[get_dir(str)]++;
		}
		free(str);
		str = get_next_line(fd);
	}
	if (!dir[0] || !dir[1] || !dir[2] || !dir[3])
		return (close(fd), 0);
	return (close(fd), (dir[0] + dir[1] + dir[2] + dir[3]) == 4);
}

static int	check_rgb(char *str)
{
	int	i;
	int	val[3];

	set_floor_ceiling(val, str);
	i = -1;
	while (++i < 3)
	{
		if (val[i] < 0 || val[i] > 255)
			return (0);
	}
	return (1);
}

static int	check_color(char *file)
{
	int		f_c[3];
	char	*str;

	ft_memset(f_c, 0, sizeof(int) * 2);
	f_c[2] = open(file, O_RDONLY);
	str = get_next_line(f_c[2]);
	while (str && f_c[2] > 2)
	{
		while (str && str[0] != 'F' && str[0] != 'C')
			free_and_gnl(&str, f_c[2]);
		if (str && (str[0] == 'F' || str[0] == 'C'))
		{
			if (!check_rgb(&str[1]))
				return (close(f_c[2]), free(str), 0);
			f_c[str[0] == 'C']++;
		}
		free_and_gnl(&str, f_c[2]);
	}
	if (!f_c[0] || !f_c[1])
		return (close(f_c[2]), free(str), 0);
	return (close(f_c[2]), free(str), f_c[0] + f_c[1] == 2);
}

int	check_arg(t_cube *cube, char *file)
{
	printf("check_arg\n");
	if (!check_all_text(cube, file))
		return (0);
	printf("arg ok\n");
	if (!check_color(file))
		return (0);
	printf("color ok\n");
	if (!check_elem(file))
		return (0);
	printf("elem ok\n");
	return (1);
}
