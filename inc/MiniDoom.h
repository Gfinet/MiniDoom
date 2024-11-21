/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniDoom.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 18:41:55 by gfinet            #+#    #+#             */
/*   Updated: 2024/11/21 18:18:08 by Gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_BONUS_H
# define CUB3D_BONUS_H

# include "./keycode.h"
# include "./Printf/ft_printf.h"
// # include "./Printf/libft/libft.h"
# include "./minilibx/mlx.h"
# include <math.h>
#include <sys/time.h>

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# define MLX_SYNC_IMAGE_WRITABLE	1
# define MLX_SYNC_WIN_FLUSH_CMD		1
# define MLX_SYNC_WIN_CMD_COMPLETED	3
# define MAX_LIFE 100
# define INVIS_WALL "0NSEWA"

# ifndef WIN_HEIGHT
#  define WIN_HEIGHT 640 //1280
# endif
# ifndef WIN_WIDTH
#  define WIN_WIDTH 1200 //2400
# endif

# define FRAME 4 //12
# define FOV 0.66 
# define JUMP_HEIGHT 80
# define LST_CHAR "012NSEWDGTA"

typedef struct s_point
{
	double	x;
	double	y;
}	t_point;

typedef struct s_mirr
{
	int	x;
	int	y;
	int	state;
}	t_mirr;

typedef struct s_img_mlx
{
	int width;
	int height;
	void *other;
}	t_img_mlx;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		height;
	int		width;
}	t_data;

typedef struct s_mini_maps
{
	t_data	maps;
	int		witdh;
	int		height;
}	t_mini_maps;

typedef struct s_part
{
	t_data	sprite;
	t_point	prev_pos;
	t_point	pos;
	t_point	dir;
	float	speed;
	int		dmg;
}	t_part;

typedef struct s_weapon
{
	t_data	*sprites;
	double	freq_atk;
	char	*name;
	int		dmg;
	char	**path;
}	t_weapon;

typedef struct s_enemy
{
	t_data	text_on;
	t_data	*spr_fr;
	t_data	*spr_bk;
	t_data	*spr_sd;
	t_point	prev_pos;
	t_point	pos;
	t_point	dir;
	t_point	hitbox;
	t_point st_dr_end;
	double	wall_dist;
	double	tmp_dist;
	double	short_dist;
	char	**path;
	int		l_r;
	int		x;
	int		path_len;
	int		max_text_fr;
	int		max_text_bk;
	int		max_text_sd;
	int		ray_hit;
	int		ray_max;
	int		draw;
	int		seen;
	int		id;
	int		hp;
	int		dmg;
	double	freq_atk;
	double	speed;
	double	fov;
}	t_enemy;

typedef struct s_maps
{
	t_mini_maps	mini;
	t_mirr		*mirr;
	char		**c_maps;
	char		**c_text;
	int			max_len;
	int			floor[3];
	int			ceil[3];
	int			m_height;
	int			nb_weap;
	int			nb_enemy;
	int			nb_mirr;
	t_weapon	*weap;
	t_enemy		*enemy;
}	t_maps;

typedef struct s_player
{
	t_point	pos;
	t_point	dir;
	t_point	prev_pos;
	t_point	pov;
	t_data	life;
	int		shoot;
	int		run;
	int		move_v;
	int		move_h;
	int		turn;
	int		use_weap;
	int		hp;
	double	angle;
	double	d_xy[2];
}	t_player;

typedef struct s_door
{
	struct s_door	*next;
	t_point			coord;
	int				open;
	int				on_going;
}	t_door;

typedef struct s_pause
{
	t_data	bg;
	t_data	title;
	t_data	resume;
	t_data	quit;
	t_data	arrow;
	int		choice;
}	t_pause;

typedef struct s_img
{
	void	*img;
	char	*path;
	int		witdh;
	int		height;
}	t_img;

typedef struct s_ray_hit {
    double	wall_dist;
    t_enemy	**enemies_hit;
    double	*enemies_dist;
    int 	nb_enemies;
}	t_ray_hit;

typedef struct s_rcdata
{
	t_point			var;
	t_point			side_dist;
	t_point			side_dist_m;
	t_point			rays;
	t_point			step;
	t_point			dest;
	t_point			center;
	int				side;
	int				d_side;
	char			hit;
	double			frameTime;
	double			oldTime;
	double			time;
	double			camerx;
	double			perp_wall_dist;
	double			m_perp_wall_dist;
	double			d_perp_wall_dist;
	int				door;
}	t_rcdata;

typedef struct s_drawdata
{
	int				tex_x;
	int				tex_y;
	double			tex_pos;
	int				line_height;
	int				pitch;
	int				draw_start;
	int				draw_end;
	int				tex_num;
	double			wall_x;
	double			step_f;
	int				mirr;
}	t_drawdata;

typedef struct s_cube
{
	void		*mlx;
	void		*win;
	t_player	*player;
	t_maps		*lvl;
	t_pause		pause_sc;
	t_data		texture[4];
	t_data		door_texture[4];
	t_data		screen;
	t_door		*doors;
	t_ray_hit	hit_data;
	double		focal_length;
	int			wall;
	int			frame;
	int			mouse;
	int			pause;
	int			m_sensi;
	int			s_mouse;
}	t_cube;

//handle_event.c
int		esc_handle(t_cube *cube);
int		fps(t_cube	*cube);
int		key_event(int keycode, t_cube *cube);
int		key_event_release(int keycode, t_cube *cube);
int		mouse_event(int x, int y, t_cube *cube);
int		mouse_press_event(int keycode, int x, int y, t_cube *cube);
int		mouse_rel_event(int keycode, int x, int y, t_cube *cube);
void	set_use_weapon(int key, t_cube *cube);

//parse_maps.c
int		get_maps(t_cube *cube, char *file);
void	fill_maps(t_maps *lvl, char *str, int fd[2]);
void	set_floor_ceiling(int fl_ce[3], char *str);
void	set_map(t_maps *lvl, char *str, int fd[2]);

//check_maps.c
int		check_map(t_cube *cube);
int		check_elem(char *file);
int		in_char_lst(char c, char *lst);

//doors
t_data	data_img(char *file, t_cube *c);
void	load_door_texture(t_cube *cube);
t_door	*init_new_door(int x, int y);
void	get_all_doors(char **map, t_cube *c);
//t_door	*find_door(t_cube *c, float x, float y);

//check_arg
int		check_arg(t_cube *cube, char *file);
int		check_elem(char *file);
int		check_line(char *str);
char	*get_text_dir(char *str);
int		get_dir(char *str);

//interactions
void	find_and_open_door(t_door *door, t_player *player);
int		impassable(char **map, t_cube *cb, int x, int y);

//background.c
void	draw_background(t_cube *cube);

//mini_maps
void	draw_mini_background(t_maps *lvl);
int		*get_ind(int i[2], int w_h[2], t_maps *lvl);
void	draw_maps(t_cube *cube);
int		make_mini(t_cube *cube, t_maps *lvl);

//mlx_img
void			get_player_pos(t_cube *cube);
void			fill_map_char(t_maps *lvl, char c);
void			draw_mini_pixel(t_maps *lvl, int w_h[2], int i[2]);
void			draw_player(t_cube *cube);
void			draw_doom(t_cube *cube);
unsigned int	get_color_from_xpm(t_data *text, int x, int y);

//movements
void	update_player(t_cube *cube, t_player *player);
void	turn(t_cube *cube, double angle, int frame);

//raycast
int		ray_in_limit(t_cube *cube, int x, int y);
void	calculate_perp_wall_dist(t_rcdata *data, int mirr);
double	fix_texture_pos(t_rcdata dt, t_player pl);
void	rcdda(t_cube *cube, char **map, t_player player);
void	raycasting(t_cube *cube);
void	set_angle(t_cube *cube, int x, int y);
double	set_delta(double ray);

void	draw_mirr_frame(t_data *screen, t_drawdata dt, int x, t_rcdata data);
void	get_door_draw_data(t_drawdata *dt, t_rcdata data, t_cube *c);
void	draw_door(t_data *screen, int x, t_rcdata data, t_cube *c);
void	update_mirror(t_rcdata *data, char **map);

void	pick_texture(t_drawdata *dr, t_rcdata dt, t_cube *cube, int door_text);
void	set_dda_ray_delta(t_rcdata *data, t_player player, int x);
void	set_side_dist_and_step(t_player p, t_rcdata *dt);
void	check_hit_target(t_rcdata *dt, char **map);
void	calculate_wall_dist(t_rcdata *data, char **map);

//parse_maps
void	set_map(t_maps *lvl, char *str, int fd[2]);
void	set_floor_ceiling(int fl_ce[3], char *str);
void	fill_maps(t_maps *lvl, char *str, int fd[2]);
int		get_maps(t_cube *cube, char *file);

//utils.c
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
void	free_and_gnl(char **str, int fd);
int		new_img(t_cube *cube, t_data *new_img, int width, int height);
void	free_maps(char **maps, int ind);
void	free_cube(t_cube *cube);
int		out_of_maps(t_maps *maps, int x, int y);
void	free_weapons(t_cube *cube);
void	free_enemy(t_cube *cube, t_enemy *adv);

//parse_weapon
int		get_weapon(t_cube *cube);
void	set_weapon(t_maps *lvl, char *str);
int		check_weapon(t_cube *cube, char *str);

//draw_weapon
int		xpm_to_img(t_cube *cube, t_data *new_img, char *name);
void	draw_weapons(t_cube *cube);

//life
int		set_life(t_cube *cube);
void	draw_life(t_cube *cube);

//pause
void	pause_screen(t_cube *cube);
int		init_pause_screen(t_cube *cube);
int		choose_pause(int keycode, t_cube *cube);
void	free_text(t_cube *cube);

//broken_mirror
void	set_mirr(t_maps *lvl);
int		count_mirr(char *line);
t_mirr	*find_mirr(t_cube *cube, int x, int y);
int		get_mirr_state(t_mirr *mirr);

//parse_enemy
t_enemy	*enemy_in_sight(t_cube *cube, t_rcdata *data);
int		check_enemy_inf(t_cube *cube, char *str);
void	set_draw_enemy(t_cube *cube, int val);
void	set_enemy(t_maps *lvl, char *str);
int		get_enemy_inf(t_cube *cube, int i);

//draw_enemy
void	draw_enemy(t_cube *cube, t_enemy *adv);
void	set_enemies_seen(t_cube *cube, int x, double wall_dist);
void	draw_enemies(t_cube *cube);
void	raycast_enemy(t_cube *cube);
void	adjust_enemy_visibility(t_cube *cube,t_enemy *adv, t_rcdata *data);

//math_utils
double	dist_ab(t_point a, t_point b);
double	ft_modf(double numb, double *integ);
#endif
