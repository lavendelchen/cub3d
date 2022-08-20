/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/02 13:34:59 by tschmitt         ###   ########.fr       */
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

enum e_texture_iterator
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

/* -------------------------------- */

typedef struct s_scene_description
{
	char	*textures[4];
	struct s_color
	{
		int			r;
		int			g;
		int			b;
	}	floor_color;
	struct s_color	ceiling_color;
	char			**map_content;
	int				starting_position[2];
	int				starting_direction;
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

typedef struct s_raycasting_calc
{
	double	camera_plane_part;
	double	ray_vector[2];
	int		tile[2];
	double	relative_distance_between_tile_borders[2];
	double	player_to_tile_border_distance[2];
	short	direction[2];
	short	hit_border;
	double	result_wall_distance;
}	t_raycasting_calc;

int		rgba(int r, int g, int b, int a);
int		parser(const char *scene_description_file_path, t_scene_description *scene_description);

#endif
