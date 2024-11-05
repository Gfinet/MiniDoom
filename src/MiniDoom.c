/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniDoom.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 18:40:12 by gfinet            #+#    #+#             */
/*   Updated: 2024/11/05 19:58:17 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/MiniDoom.h"

static int	check_format(char *file)
{
	size_t	i;

	i = ft_strlen(file);
	while (file && i && file[i] != '.')
		i--;
	if (!file || ft_strncmp(&file[i], ".cub", 5))
		return (0);
	return (1);
}

static int	init_cube(t_cube *cube, t_player *play, t_maps *level)
{
	*cube = (t_cube){0};
	*play = (t_player){0};
	*level = (t_maps){0};
	cube->screen = (t_data){0};
	cube->frame = FRAME;
	cube->mlx = mlx_init();
	if (!cube->mlx)
		return (0);
	cube->win = mlx_new_window(cube->mlx, WIN_WIDTH, WIN_HEIGHT, "DOOM3D");
	if (!cube->win)
		return (0);
	//load_door_texture(cube);
	cube->lvl = level;
	cube->player = play;
	cube->m_sensi = 10;
	cube->s_mouse = 1;
	//mlx_mouse_hide();
	return (1);
}

static int	get_textures(t_cube *cube)
{
	int		i;
	t_data	*txt;

	txt = cube->texture;
	i = -1;
	while (++i < 4)
		if (!xpm_to_img(cube, &txt[i], cube->lvl->c_text[i]))
			return (-i);
	if (!get_weapon(cube))
		return (0);
	if (!get_enemy_inf(cube))
		return (0);
	if (!new_img(cube, &cube->screen, WIN_WIDTH, WIN_HEIGHT))
		return (0);
	return (1);
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
	t_maps		level;
	t_player	player;

	if (argc != 2)
		return (write(2, ERROR_ARG, 14), 0);
	if (!check_format(argv[1]))
		return (write(2, ERROR_FRM, 17), 0);
	if (!init_cube(&cube, &player, &level))
		return (write(2, ERROR_MAL, 19), free_cube(&cube), 0);
	if (!get_maps(&cube, argv[1]))
		return (write(2, ERROR_MAP, 15), free_cube(&cube), 0);
	if (!get_textures(&cube))
		return (write(2, ERROR_TXT, 19), free_cube(&cube), 0);
	if (!make_mini(&cube, &level))
		return (write(2, ERROR_MMP, 34), free_cube(&cube), 0);
	if (!set_life(&cube))
		return (write(2, ERROR_HP, 30), free_cube(&cube), 0);
	if (!init_pause_screen(&cube))
		return (write(2, ERROR_PSC, 38), free_cube(&cube), 0);
	game_loop_init(&cube);
	return (0);
}
