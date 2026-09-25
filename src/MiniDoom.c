/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniDoom.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 18:40:12 by gfinet            #+#    #+#             */
/*   Updated: 2026/09/25 14:01:58 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

static int	check_file_format(char *file)
{
	size_t	i;

	i = ft_strlen(file);
	while (file && i && file[i] != '.')
		i--;
	if (!file || ft_strncmp(&file[i], ".cub", 5))
		return (0);
	return (1);
}

static void write_error(char *msg)
{
	write(2, msg, ft_strlen(msg));
}

static int	init_cube(t_cube *cube, t_player *play, t_lvl *level)
{
	*cube = (t_cube){0};
	*play = (t_player){0};
	*level = (t_lvl){0};
	cube->screen = (t_data){0};
	if (!xpm_to_img(cube, &cube->no_sprite, "./texture/no_sprites.xpm"))
		return 0;
	cube->wall_dist = 0;
	cube->stop = 0;
	cube->frame = FRAME;
	cube->mlx = mlx_init();
	if (!cube->mlx)
		return (0);
	cube->win = mlx_new_window(cube->mlx, WIN_WIDTH, WIN_HEIGHT, "DOOM3D");
	if (!cube->win)
		return (0);
	//load_door_texture(cube);
	cube->lvl = level;
	cube->lvl->cube = cube;
	cube->player = play;
	cube->m_sensi = 10;
	cube->s_mouse = 1;
	cube->focal_length = (WIN_WIDTH / 2) / tan(FOV / 2);
	pthread_mutex_init(&cube->playpos_mutex, 0);
	pthread_mutex_init(&cube->stop_mutex, 0);
	pthread_mutex_init(&cube->pause_mutex, 0);
	//mlx_mouse_hide();
	return (1);
}

static int	get_textures(t_cube *cube)
{
	t_data	*txt;

	txt = cube->texture;
	for (int i = 0; i < 4; i++)
		if (!xpm_to_img(cube, &txt[i], cube->lvl->c_text[i]))
			return (-i);
	for (int i = 0; i < cube->lvl->nb_weap; i++)
		if (!get_weapon_inf(cube, i))
			return (0);
	for (int i = 0; i < cube->lvl->nb_enemy_type; i++)
		if (!get_enemy_inf(cube, i))
			return (0);
	if (!new_img(cube, &cube->screen, WIN_WIDTH, WIN_HEIGHT))
		return (0);
	return (1);
}

void	draw_doom(t_cube *cube)
{
	mlx_clear_window(cube->mlx, cube->win);
	raycasting(cube);
	draw_player(cube);
	draw_enemies(cube);
	draw_ui(cube);
	// red_filter(&cube->screen, 0.5);
	mlx_put_image_to_window(cube->mlx, cube->win, cube->screen.img, 0, 0);
}

static void	game_loop_init(t_cube *cube)
{
	draw_doom(cube);
	mlx_hook(cube->win, 17, 0, &esc_handle, cube);
	mlx_hook(cube->win, 2, 0, &key_event, cube);
	mlx_hook(cube->win, 3, 10, &key_event_release, cube);
	mlx_hook(cube->win, 6, 0, &mouse_event, cube);
	mlx_hook(cube->win, 4, (1L << 2), &mouse_press_event, cube);
	mlx_hook(cube->win, 5, (1L << 3), &mouse_rel_event, cube);
	mlx_loop_hook(cube->mlx, &fps, cube);
	mlx_loop(cube->mlx);
}

int	main(int argc, char **argv)
{
	t_cube		cube;
	t_lvl		level;
	t_player	player;

	if (argc != 2)
		return (write_error(ERROR_ARG), 0);
	if (!check_file_format(argv[1]))
		return (write_error(ERROR_FRM), 0);
	if (!init_cube(&cube, &player, &level))
		return (write_error(ERROR_MAL), free_cube(&cube), 0);
	if (!get_maps(&cube, argv[1]))
		return (write_error(ERROR_MAP), free_cube(&cube), 0);
	if (!get_textures(&cube))
		return (write_error(ERROR_TXT), free_cube(&cube), 0);
	if (!make_mini(&cube, &level))
		return (write_error(ERROR_MMP), free_cube(&cube), 0);
	if (!set_life(&cube))
		return (write_error(ERROR_HP), free_cube(&cube), 0);
	if (!make_ui(&cube))
		return (write_error(ERROR_UI), free_cube(&cube), 0);
	if (!init_pause_screen(&cube))
		return (write_error(ERROR_PSC), free_cube(&cube), 0);
	if (!launch_eneny_thread(&cube))
		return (write_error(ERROR_TH), free_cube(&cube), 0);
	game_loop_init(&cube);
	return (0);
}
