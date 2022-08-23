/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/23 21:53:53 by tschmitt         ###   ########.fr       */
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
int		rgba(int r, int g, int b, int a);

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

/**
 * @brief remove the [0.] and you have the angle of the field of vision
 */
# define FOV 0.66 
/**
 * @brief value 1 means 1 tile will have equal wall height and width. 
 * @param higher wallheight will make higher walls
 * @param lower wallheight will make smaller ones.
 */
# define WALLHEIGHT 1

/**
 * @brief multiplier for movement velocity, make higher for faster movement
 */
# define MOVESPEED 0.03
/**
 * @brief mltiplier for rotation velocity, make higher for faster rotation
 */
# define ROTSPEED 0.02

/* -------------------------------- */

typedef struct s_player
{
	int					position[2];
	enum e_direction	direction;
}	t_player;

/**
 * @brief 
 * @param map_content x and y start at upper left corner.
 * that means y increases in south direction,
 * and x increases in east direction.
 * 
 */
typedef struct s_scene_description
{
	char		*textures[4];
	int			floor_color;
	int			ceiling_color;
	char		**map_content;
	t_player	player;
}	t_scene_description;

/**
 * @brief Structure that holds multiple Vectors
 * @param player_position posX / posY
 * @param player_direction dirX / dirY
 * @param camera_plane planeX / planeY
 */
typedef struct s_vectors
{
	double	player_position[2];
	double	player_direction[2];
	double	camera_plane[2];
}	t_vectors;

typedef struct s_game
{
	mlx_t			*mlx_ptr;
	mlx_image_t		*mlx_img;
	mlx_texture_t	*wall[4];
	t_vectors		vectors;
}	t_game;

typedef struct s_bundle
{
	t_scene_description	*scene_desc;
	t_game				*game;
}	t_bundle;

/**
 * @brief 
 * @param camera_plane_part cameraX
 * @param ray_vector rayDirX / rayDirY
 * @param tile mapX / mapY
 * @param tile_border_distance deltaDistX / deltaDistY
 * @param player_to_tile_border sideDistX / sideDistY
 * @param direction stepX / stepY
 * @param hit_border side | NO_SO = 0, WE_EA = 1
 * @param potential_wall_direction
 * @param wall_direction
 * @param wall_distance perpWallDist
 * 
 */
typedef struct s_raycasting_calc
{
	double	camera_plane_part;
	double	ray_vector[2];
	int		tile[2];
	double	tile_border_distance[2];
	double	player_to_tile_border[2];
	short	direction[2];
	short	hit_border;
	short	potential_wall_direction[2];
	short	wall_direction;
	double	wall_distance;
}	t_raycasting_calc;

/**
 * @brief 
 * @param wall_height lineHeight
 * @param first_pixel
 * @param last_pixel
 * @param wall_hitpoint wallX
 * @param step
 * @param texture_pixel texX
 * @param texture_position
 * 
 */
typedef struct s_texture_calc
{
	int		wall_height;
	int		first_pixel;
	int		last_pixel;
	double	wall_hitpoint;
	double	step;
	int		texture_pixel[2];
	double	texture_position;
}	t_texture_calc;

typedef struct s_square_data
{
	mlx_image_t	*mlx_img;
	int			start_pixel[2];
	int			length[2];
	int			color;
}	t_square_data;

int		rgba(int r, int g, int b, int a);
int		parser(
			const char *scene_description_file_path,
			t_scene_description *scene_description
			);

void	init_game(t_game *game, t_scene_description *scene_desc);
void	raycasting_loop(void *bundle);

void	init_wall_hit_calc(t_raycasting_calc *cast, t_game *game);
void	wall_hit_calc(t_raycasting_calc *cast, t_scene_description *scene_desc);

void	draw_wall(t_raycasting_calc *cast, t_game *game, int ray_iter);

/* MOVEMENT */
/* move */
void	check_movement(struct s_vectors *vectors, char **map, mlx_t *mlx_ptr);
/* rotate */
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
