/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/21 23:11:17 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/* mlx library */
# include "../lib/MLX42/include/MLX42/MLX42.h"

/* Libft Library */
# include "../lib/libft/include/libft.h"

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <memory.h>
# include <math.h>


/* RGBA / UTILS */
int     rgba(int r, int g, int b, int a);

/* Parser */

enum e_direction
{
	NO = 0,
	SO = 1,
	WE = 2,
	EA = 3
};

enum e_grid
{
	X = 0,
	Y = 1
};

enum e_borders
{
	NO_SO = 0,
	WE_EA = 1
};

/* Modifiable stuff to modify the game! */

enum e_screensize
{
	SCREENWIDTH = 1920,
	SCREENHEIGHT = 1080
};

#define FOV 0.66 //remove the [0.] and you have the angle of the field of vision
#define WALLHEIGHT 1 // value 1 means 1 tile will have equal wall height and width. higher wallheight will make higher walls, lower wallheight will make smaller ones.

#define MOVESPEED 0.005 // multiplier for movement velocity, make higher for faster movement
#define ROTSPEED 0.003 // mltiplier for rotation velocity, make higher for faster rotation

/* -------------------------------- */

typedef struct s_player
{
	int					position[2];
	enum e_direction	direction;
}	t_player;

typedef struct s_scene_description
{
	char		*textures[4];
	int			floor_color;
	int			ceiling_color;
	char		**map_content;
	t_player	player;
}	t_scene_description;

typedef struct s_game
{
	mlx_t		*mlx_ptr;
	mlx_image_t	*mlx_img; // for now
	struct s_vectors
	{
		double	player_position[2]; // do we start at the corner of a tile
		double	player_direction[2];
		double	camera_plane[2];
	}	vectors;
	double	current_frame_time;
	double	last_frame_time;
}	t_game;

typedef struct s_bundle
{
	t_scene_description	*scene_desc;
	t_game				*game;
}	t_bundle;

typedef struct s_raycasting_calc
{
	double	camera_plane_part;
	double	ray_vector[2];
	int		tile[2];
	double	tile_border_distance[2];
	double	player_to_tile_border[2];
	short	direction[2];
	short	hit_border;
	double	result_wall_distance;
}	t_raycasting_calc;

typedef struct s_square_data
{
	mlx_image_t *mlx_img;
	int			start_pixel[2];
	int			length[2];
	int			color;
}	t_square_data;

int		rgba(int r, int g, int b, int a);
int		parser(const char *scene_description_file_path, t_scene_description *scene_description);

/* UTILS */
/* Close Utils */
void	close_at_esc(mlx_key_data_t key_data, void *arg);
void	free_at_window_close(void *arg);

#endif
