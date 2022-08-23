/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/22 22:07:56 by shaas            ###   ########.fr       */
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

#define MOVESPEED 0.03 // multiplier for movement velocity, make higher for faster movement
#define ROTSPEED 0.02 // mltiplier for rotation velocity, make higher for faster rotation

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
	mlx_t			*mlx_ptr;
	mlx_image_t		*mlx_img; // for now
	mlx_texture_t	*wall[4];
	struct s_vectors
	{
		double	player_position[2];		// posX / posY
		double	player_direction[2];	// dirX / dirY
		double	camera_plane[2];		// planeX / planeY
	}	vectors;
}	t_game;

typedef struct s_bundle
{
	t_scene_description	*scene_desc;
	t_game				*game;
}	t_bundle;

typedef struct s_raycasting_calc
{
	double	camera_plane_part;			// cameraX
	double	ray_vector[2];				// rayDirX / rayDirY
	int		tile[2];					// mapX / mapY
	double	tile_border_distance[2];	// deltaDistX / deltaDistY
	double	player_to_tile_border[2];	// sideDistX / sideDistY
	short	direction[2];				// stepX / stepY
	short	hit_border;					// side | NO_SO = 0, WE_EA = 1
	short	potential_wall_direction[2];
	short	wall_direction;
	double	wall_distance;		// perpWallDist
}	t_raycasting_calc;

typedef struct s_texture_calc
{
	int		wall_height; //lineHeight
	int		first_pixel;
	int		last_pixel;
	double	wall_hitpoint; //wallX
	double	step;
	int		texture_pixel[2]; //texX
	double	texture_position;
}	t_texture_calc;

typedef struct s_square_data
{
	mlx_image_t *mlx_img;
	int			start_pixel[2];
	int			length[2];
	int			color;
}	t_square_data;

int		rgba(int r, int g, int b, int a);
int		parser(const char *scene_description_file_path, t_scene_description *scene_description);

void	print_data(struct s_vectors *vectors);

void	init_game(t_game *game, t_scene_description *scene_desc);

void	check_left_right_movement(
		struct s_vectors *vectors, char **map, mlx_t *mlx_ptr);
void	check_forward_back_movement(
		struct s_vectors *vectors, char **map, mlx_t *mlx_ptr);
void	check_rotation(struct s_vectors *vectors, mlx_t *mlx_ptr);

/* UTILS */
/* Close Utils */
void	close_at_esc(mlx_key_data_t key_data, void *arg);
void	free_at_window_close(void *arg);
/* Draw Utils */
void	paste_png(t_game *game, char *png);
void	put_square(t_square_data *square);
void	paste_texture(mlx_texture_t *tex, t_game *game);

#endif
