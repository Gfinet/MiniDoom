/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_arg2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 21:54:06 by gfinet            #+#    #+#             */
/*   Updated: 2024/09/12 11:50:28 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

int	get_dir(char *str)
{
	if (!ft_strncmp(str, "NO", 2))
		return (0);
	else if (!ft_strncmp(str, "SO", 2))
		return (1);
	else if (!ft_strncmp(str, "WE", 2))
		return (2);
	else if (!ft_strncmp(str, "EA", 2))
		return (3);
	else if (!ft_strncmp(str, "G ", 2))
		return (4);
	else
		return (-1000);
}

char	*get_text_dir(char *str)
{
	size_t	len;
	int		start;

	start = 0;
	while (str[start] == ' ')
		start++;
	len = ft_strlen(&str[start]) - 1;
	return (ft_substr(str, start, len));
}

int	check_line(char *str)
{
	int			len;
	int			i;
	static int	flag = 0;
	static int	count = 0;

	i = -1;
	len = (int)ft_strlen(str);
	while (++i < len - 1)
	{
		if (str[i] != ' ' && !in_char_lst(str[i], LST_CHAR))
			flag += 1;
		if (str[i] == 'N' || str[i] == 'S' || str[i] == 'E' || str[i] == 'W')
			count += 1;
	}
	return (flag + count == 1);
}

int	check_elem(char *file)
{
	int		flag;
	int		nb_info;
	int		fd;
	char	*str;

	fd = open(file, O_RDONLY);
	str = get_next_line(fd);
	nb_info = 0;
	while (str && nb_info < 6)
	{
		if (str[0] == 'F' || str[0] == 'C' || str[0] == 'N'
			|| str[0] == 'E' || str[0] == 'S' || str[0] == 'W')
			nb_info++;
		free_and_gnl(&str, fd);
	}
	while (str && str[0] == '\n')
		free_and_gnl(&str, fd);
	while (str)
	{
		flag = check_line(str);
		free_and_gnl(&str, fd);
	}
	return (free(str), flag);
}
