/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 18:03:09 by gfinet            #+#    #+#             */
/*   Updated: 2024/02/06 23:55:34 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_strdup_char(const char *src, int c)
{
	char		*p;
	size_t		len;
	size_t		i;

	if (src == NULL)
		return (0);
	len = 0;
	while (src[len] != c && src[len])
		len++;
	if (c == '\n')
		len++;
	p = (char *)malloc((len + 1) * sizeof(char));
	if (p == NULL)
		return (0);
	i = 0;
	while ((&src[i]) != p + 1 && src[i] && i < len)
	{
		p[i] = src[i];
		i++;
	}
	p[len] = '\0';
	return (p);
}

int	read_one_line(char *rest[], int *byte, int fd)
{
	char		*buff;

	buff = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (buff == NULL)
		return ((free_all_gnl(&rest[fd], 0)) != NULL);
	*byte = read(fd, buff, BUFFER_SIZE);
	while (*byte > 0)
	{
		buff[*byte] = '\0';
		rest[fd] = ft_stradd(rest[fd], buff);
		if (!rest[fd])
			return ((free_all_gnl(&rest[fd], &buff)) != NULL);
		if (ft_strchr(buff, '\n'))
			break ;
		*byte = read(fd, buff, BUFFER_SIZE);
	}
	free(buff);
	return (1);
}

void	empty_res(char *rest[], char **result, int byte, int fd)
{
	char	*p;

	if (byte == -1)
	{
		free(rest[fd]);
		rest[fd] = NULL;
	}
	else
	{
		p = ft_strchr(rest[fd], '\n');
		*result = ft_strdup_char(rest[fd], '\n');
		if (*result == NULL)
		{
			free(rest[fd]);
			rest[fd] = NULL;
		}
		if (p && rest[fd] != NULL)
		{
			p = ft_strdup(p + 1);
			free(rest[fd]);
			if (p == NULL && *result != NULL)
				free_all_gnl(&(*result), 0);
			rest[fd] = p;
		}
	}
}

char	*end_call(char *rest[], int byte, int fd)
{
	char	*result;

	result = NULL;
	if (rest[fd] == NULL)
		return (result);
	if (ft_strchr(rest[fd], '\n'))
		empty_res(rest, &result, byte, fd);
	else
	{
		if (rest[fd] != NULL && rest[fd][0] != '\0' && byte != -1)
			result = ft_strdup(rest[fd]);
		free(rest[fd]);
		rest[fd] = NULL;
	}
	return (result);
}

char	*get_next_line(int fd)
{
	static char	*rest[1024];
	int			byte;

	if (BUFFER_SIZE < 1 || fd < 0 || BUFFER_SIZE >= INT32_MAX || fd > 1023)
		return (NULL);
	if (rest[fd] && ft_strchr(rest[fd], '\n'))
		return (end_call(rest, 1, fd));
	if (read_one_line(rest, &byte, fd))
		return (end_call(rest, byte, fd));
	else
		return (NULL);
}
