/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_img.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 22:03:11 by gfinet            #+#    #+#             */
/*   Updated: 2026/09/23 20:49:09 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

void	draw_mini_pixel(t_lvl *lvl, int w_h[2], int i[2])
{
	int	x;
	int	y;

	x = i[1] + WIN_WIDTH / 5 / (lvl->mini.witdh);
	y = i[0] + WIN_HEIGHT / 5 / (lvl->mini.height);
	if (lvl->c_maps[w_h[0]][w_h[1]] == '1'
		|| lvl->c_maps[w_h[0]][w_h[1]] == '2')
		my_mlx_pixel_put(&lvl->mini.maps, x, y, WHITE + 0x55000000);
	else if (lvl->c_maps[w_h[0]][w_h[1]] == '0')
		my_mlx_pixel_put(&lvl->mini.maps, x, y, GREEN);
	else if (lvl->c_maps[w_h[0]][w_h[1]] == 'D')
		my_mlx_pixel_put(&lvl->mini.maps, x, y, 0x55880088);
	else if (lvl->c_maps[w_h[0]][w_h[1]] == '.'
		|| lvl->c_maps[w_h[0]][w_h[1]] == 0)
		my_mlx_pixel_put(&lvl->mini.maps, x, y, BLUE + 0x55000000);
	else
		my_mlx_pixel_put(&lvl->mini.maps, x, y, GREEN);
}

int	xpm_to_img(t_cube *cube, t_data *new_img, char *name)
{
	new_img->img = mlx_xpm_file_to_image(cube->mlx, name,
			&new_img->width, &new_img->height);
	if (!new_img->img)
		return (0);
	new_img->addr = mlx_get_data_addr(new_img->img, &new_img->bits_per_pixel,
			&new_img->line_length, &new_img->endian);
	return (1);
}

int	new_img(t_cube *cube, t_data *new_img, int width, int height)
{
	new_img->img = mlx_new_image(cube->mlx, width, height);
	if (!new_img->img)
		return (0);
	new_img->addr = mlx_get_data_addr(new_img->img, &new_img->bits_per_pixel,
			&new_img->line_length, &new_img->endian);
	new_img->width = width;
	new_img->height = height;
	return (1);
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;

}

unsigned int get_color_from_xpm(t_data *text, int x, int y)
{
	int				pixel;
	unsigned int	color;

	pixel = (int)(text->line_length * y / 4 + x * (text->bits_per_pixel / 32));
	color = *((unsigned int *)text->addr + pixel);
	return (color);
}

void mlx_image_to_image(t_data *dest, t_data *src, int pos_x, int pos_y)
{
    int             x;
    int             y;
    unsigned int    color;
    char            *src_pixel;

    if (!dest || !src || !dest->addr || !src->addr)
		return ;
	
	// printf("h w %d %d\n", src->height, src->width);
    for (y = 0; y < src->height; y++)
    {
        if (pos_y + y < 0 || pos_y + y >= dest->height)
			continue ;
        for (x = 0; x < src->width; x++)
        {
            if (pos_x + x < 0 || pos_x + x >= dest->width)
                continue ;

            src_pixel = src->addr + (y * src->line_length + x * (src->bits_per_pixel / 8));
            color = *(unsigned int *)src_pixel;
            if ((color & 0x00FFFFFF) == 0x000000)
                continue ;
            my_mlx_pixel_put(dest, pos_x + x, pos_y + y, color);
        }
    }
}

void red_filter(t_data *img, double intensity)
{
	int 			x, y;
	unsigned int    color;
    char            *pixel;
	uint8_t         r, g, b;

	if (intensity <= 0.0 || !img->addr)
		return ;
    if (intensity > 1.0)
		intensity = 1.0;

	for (y = 0; y < img->height; y++)
    {
        for (x = 0; x < img->width; x++)
        {
			pixel = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
            color = *(unsigned int *)pixel;
			r = (color >> 16) & 0xFF;
            g = (color >> 8) & 0xFF;
            b = color & 0xFF;

            r = (uint8_t)(r + (255 - r) * intensity);
            g = (uint8_t)(g * (1.0 - intensity * 0.5));
            b = (uint8_t)(b * (1.0 - intensity * 0.5));
			*(unsigned int *)pixel = (r << 16) | (g << 8) | b;
		}
	}
}