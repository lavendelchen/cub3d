/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/21 23:09:48 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

bool	init_directions_no_so(int starting_direction, struct s_vectors *vectors)
{
	if (starting_direction == NO)
	{
		vectors->player_direction[X] = 0;
		vectors->player_direction[Y] = -1; //now up is - and down is + since that is how the map works i think
		vectors->camera_plane[X] = FOV;
		vectors->camera_plane[Y] = 0;
		return (true);
	}
	else if (starting_direction == SO)
	{
		vectors->player_direction[X] = 0;
		vectors->player_direction[Y] = 1;
		vectors->camera_plane[X] = FOV * -1;
		vectors->camera_plane[Y] = 0;
		return (true);
	}
	return (false);
}

void	init_directions_we_ea(int starting_direction, struct s_vectors *vectors)
{
	if (starting_direction == WE)
	{
		vectors->player_direction[X] = -1;
		vectors->player_direction[Y] = 0;
		vectors->camera_plane[X] = 0;
		vectors->camera_plane[Y] = FOV * -1;
	}
	else if (starting_direction == EA)
	{
		vectors->player_direction[X] = 1;
		vectors->player_direction[Y] = 0;
		vectors->camera_plane[X] = 0;
		vectors->camera_plane[Y] = FOV;
	}
}

void	init_game(t_game *game, t_scene_description *scene_desc)
{
	game->vectors.player_position[X]
		= scene_desc->player.position[X] + 0.5; // so that we are in the middle of that square
	game->vectors.player_position[Y]
		= scene_desc->player.position[Y] + 0.5; // same as above // or minus - ?
	if (!init_directions_no_so(
			scene_desc->player.direction, &(game->vectors)))
		init_directions_we_ea(
			scene_desc->player.direction, &(game->vectors));
	game->current_frame_time = 0;
	game->last_frame_time = 0;
}

void	init_wall_hit_calc(t_raycasting_calc *cast, t_game *game)
{
	if (cast->ray_vector[X] < 0)
	{
		cast->direction[X] = -1;
		cast->player_to_tile_border[X] = (game->vectors.player_position[X]
				- cast->tile[X]) * cast->tile_border_distance[X];
	}
	else
	{
		cast->direction[X] = 1;
		cast->player_to_tile_border[X] = (cast->tile[X] + 1.0 \
			- game->vectors.player_position[X]) * cast->tile_border_distance[X];
	}
	if (cast->ray_vector[Y] < 0)
	{
		cast->direction[Y] = -1;
		cast->player_to_tile_border[Y] = (game->vectors.player_position[Y]
				- cast->tile[Y]) * cast->tile_border_distance[Y];
	}
	else
	{
		cast->direction[Y] = 1;
		cast->player_to_tile_border[Y] = (cast->tile[Y] + 1.0 \
			- game->vectors.player_position[Y]) * cast->tile_border_distance[Y];
	}
}

void	wall_hit_calc(t_raycasting_calc *cast, t_scene_description *scene_desc)
{
	while (true)
	{
		if (cast->player_to_tile_border[X] < cast->player_to_tile_border[Y])
		{
			cast->player_to_tile_border[X] += cast->tile_border_distance[X];
			cast->tile[X] += cast->direction[X];
			cast->hit_border = NO_SO; //correct?
		}
		else
		{
			cast->player_to_tile_border[Y] += cast->tile_border_distance[Y];
			cast->tile[Y] += cast->direction[Y];
			cast->hit_border = WE_EA; //correct?
		}
		if (scene_desc->map_content[cast->tile[Y]][cast->tile[X]] == '1')
			break ;
	}
	if (cast->hit_border == NO_SO)
		cast->result_wall_distance = cast->player_to_tile_border[X]
			- cast->tile_border_distance[X];
	else
		cast->result_wall_distance = cast->player_to_tile_border[Y]
			- cast->tile_border_distance[Y];
}

void	put_square(t_square_data *square)
{
	int	iter[2];

	iter[Y] = 0;
	while (iter[Y] < square->length[Y]
		&& iter[Y] + square->start_pixel[Y] < (int)square->mlx_img->height)
	{
		iter[X] = 0;
		while (iter[X] < square->length[X]
			&& iter[X] + square->start_pixel[X] < (int)square->mlx_img->width)
		{
			mlx_put_pixel(square->mlx_img, square->start_pixel[X] + iter[X],
				square->start_pixel[Y] + iter[Y], square->color);
			iter[X]++;
		}
		iter[Y]++;
	}
}

void	draw_wall(t_raycasting_calc *cast, t_game *game,
					t_scene_description *scene_desc, int ray_iter)
{
	int	wall_height;
	int	first_pixel;
	int	last_pixel;

	wall_height = (int)(SCREENHEIGHT * WALLHEIGHT) / cast->result_wall_distance;
	first_pixel = (SCREENHEIGHT / 2) - (wall_height / 2);
	if (first_pixel < 0)
		first_pixel = 0;
	last_pixel = (SCREENHEIGHT / 2) + (wall_height / 2);
	if (last_pixel >= SCREENHEIGHT)
		last_pixel = SCREENHEIGHT - 1;
	//later textures, now just some color
	while (first_pixel <= last_pixel)
	{
		mlx_put_pixel(game->mlx_img, ray_iter, first_pixel, 0xFF0000EE);
		first_pixel++;
	}
	(void)scene_desc;
}

void	ray_calc(t_raycasting_calc *cast, t_game *game, int ray_iter)
{
	cast->camera_plane_part = (ray_iter * 2 / SCREENWIDTH) - 1;
	cast->ray_vector[X] = game->vectors.player_direction[X]
		+ (game->vectors.camera_plane[X] * cast->camera_plane_part);
	cast->ray_vector[Y] = game->vectors.player_direction[Y]
		+ (game->vectors.camera_plane[Y] * cast->camera_plane_part);
	cast->tile[X] = (int)game->vectors.player_position[X];
	cast->tile[Y] = (int)game->vectors.player_position[Y];
	if (cast->ray_vector[X] == 0)
		cast->tile_border_distance[X] = INFINITY;
	else
		cast->tile_border_distance[X] = fabs(1 / cast->ray_vector[X]);
	if (cast->ray_vector[Y] == 0)
		cast->tile_border_distance[Y] = INFINITY;
	else
		cast->tile_border_distance[Y] = fabs(1 / cast->ray_vector[Y]);
}

void	put_floor_ceiling(mlx_image_t *mlx_img, t_scene_description *scene_desc)
{
	t_square_data	square;

	square.mlx_img = mlx_img;
	square.length[X] = SCREENWIDTH;
	square.length[Y] = SCREENHEIGHT / 2;
	square.start_pixel[X] = 0;
	square.start_pixel[Y] = 0;
	square.color = scene_desc->ceiling_color;
	put_square(&square);
	square.start_pixel[Y] = SCREENHEIGHT / 2;
	square.color = scene_desc->floor_color;
	put_square(&square);
}

void	print_data(struct s_vectors *vectors)
{
	printf("NEW ITERATION\n");
	printf("Player position X: [%f]\n", vectors->player_position[X]);
	printf("Player position Y: [%f]\n", vectors->player_position[Y]);
	printf("Player direction X: [%f]\n", vectors->player_direction[X]);
	printf("Player direction Y: [%f]\n", vectors->player_direction[Y]);
	printf("Camera plane X: [%f]\n", vectors->camera_plane[X]);
	printf("Camera plane Y: [%f]\n", vectors->camera_plane[Y]);
}

void	check_left_right_movement(
	struct s_vectors *vectors, char **map, mlx_t *mlx_ptr)
{
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_D))
	{
		if (map[(int)(vectors->player_position[X] - (vectors->player_direction[Y]
		* MOVESPEED))][(int)vectors->player_position[Y]] != '1')
			vectors->player_position[X]
			-= vectors->player_direction[Y] * MOVESPEED;
		if (map[(int)vectors->player_position[X]]
		[(int)(vectors->player_position[Y] + (vectors->player_direction[X]
		* MOVESPEED))] != '1')
			vectors->player_position[Y]
			+= vectors->player_direction[X] * MOVESPEED;
		print_data(vectors);
	}
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_A))
	{
		if (map[(int)(vectors->player_position[X] + (vectors->player_direction[Y]
		* MOVESPEED))][(int)vectors->player_position[Y]] != '1')
			vectors->player_position[X]
			+= vectors->player_direction[Y] * MOVESPEED;
		if (map[(int)vectors->player_position[X]]
		[(int)(vectors->player_position[Y] - (vectors->player_direction[X]
		* MOVESPEED))] != '1')
			vectors->player_position[Y]
			-= vectors->player_direction[X] * MOVESPEED;
		print_data(vectors);
	}
}

void	check_forward_back_movement(
	struct s_vectors *vectors, char **map, mlx_t *mlx_ptr)
{
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_W))
	{
		if (map[(int)(vectors->player_position[X] + (vectors->player_direction[X]
		* MOVESPEED))][(int)vectors->player_position[Y]] != '1')
			vectors->player_position[X]
			+= vectors->player_direction[X] * MOVESPEED;
		if (map[(int)vectors->player_position[X]]
		[(int)(vectors->player_position[Y] + (vectors->player_direction[Y]
		* MOVESPEED))] != '1')
			vectors->player_position[Y]
			+= vectors->player_direction[Y] * MOVESPEED;
		print_data(vectors);
	}
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_S))
	{
		if (map[(int)(vectors->player_position[X] - (vectors->player_direction[X]
		* MOVESPEED))][(int)vectors->player_position[Y]] != '1')
			vectors->player_position[X]
			-= vectors->player_direction[X] * MOVESPEED;
		if (map[(int)vectors->player_position[X]]
		[(int)(vectors->player_position[Y] - (vectors->player_direction[Y]
		* MOVESPEED))] != '1')
			vectors->player_position[Y]
			-= vectors->player_direction[Y] * MOVESPEED;
		print_data(vectors);
	}
}

void	check_rotation(struct s_vectors *vectors, mlx_t *mlx_ptr)
{
	double	old_player_dir[1];
	double	old_camera_plane[1];

	old_player_dir[X] = vectors->player_direction[X];
	old_camera_plane[X] = vectors->camera_plane[X];
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_LEFT))
	{
		vectors->player_direction[X] = vectors->player_direction[X] * cos(-ROTSPEED)
		- vectors->player_direction[Y] * sin(-ROTSPEED);
		vectors->player_direction[Y] = old_player_dir[X] * sin(-ROTSPEED)
		+ vectors->player_direction[Y] * cos(-ROTSPEED);
		vectors->camera_plane[X] = vectors->camera_plane[X]
		* cos(-ROTSPEED) - vectors->camera_plane[Y] * sin(-ROTSPEED);
		vectors->camera_plane[Y] = old_camera_plane[X] * sin(-ROTSPEED)
		+ vectors->camera_plane[Y] * cos(-ROTSPEED);
		print_data(vectors);
	}
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_RIGHT))
	{
		vectors->player_direction[X] = vectors->player_direction[X] * cos(ROTSPEED)
		- vectors->player_direction[Y] * sin(ROTSPEED);
		vectors->player_direction[Y] = old_player_dir[X] * sin(ROTSPEED)
		+ vectors->player_direction[Y] * cos(ROTSPEED);
		vectors->camera_plane[X] = vectors->camera_plane[X]
		* cos(ROTSPEED) - vectors->camera_plane[Y] * sin(ROTSPEED);
		vectors->camera_plane[Y] = old_camera_plane[X] * sin(ROTSPEED)
		+ vectors->camera_plane[Y] * cos(ROTSPEED);
		print_data(vectors);
	}
}

void	raycasting_loop(void *bundle)
{
	static int					ray_iter;
	static t_raycasting_calc	cast;
	t_game						*game;
	t_scene_description			*scene_desc;

	game = ((t_bundle *)bundle)->game;
	scene_desc = ((t_bundle *)bundle)->scene_desc;
	put_floor_ceiling(game->mlx_img, scene_desc);
	ray_iter = 0;
	while (ray_iter < SCREENWIDTH)
	{
		ray_calc(&cast, game, ray_iter);
		init_wall_hit_calc(&cast, game);
		wall_hit_calc(&cast, scene_desc);
		draw_wall(&cast, game, scene_desc, ray_iter);
		ray_iter++;
	}
	check_forward_back_movement(&(game->vectors), scene_desc->map_content, game->mlx_ptr);
	check_left_right_movement(&(game->vectors), scene_desc->map_content, game->mlx_ptr);
	check_rotation(&(game->vectors), game->mlx_ptr);
}

int	main(int argc, const char *argv[])
{
	t_scene_description	scene_desc;
	t_game				game;
	t_bundle			bundle;

	if (argc != 2)
	{
		ft_putstr_fd("Error\nOne Argument Needed\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (parser(argv[1], &scene_desc) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	bundle.game = &game;
	bundle.scene_desc = &scene_desc;
	init_game(&game, &scene_desc);
	print_data(&(game.vectors));
	game.mlx_ptr = mlx_init(SCREENWIDTH, SCREENHEIGHT, "🌈RainbowCube🌈", false);
	game.mlx_img = mlx_new_image(game.mlx_ptr, SCREENWIDTH, SCREENHEIGHT);
	mlx_image_to_window(game.mlx_ptr, game.mlx_img, 0, 0);
	mlx_loop_hook(game.mlx_ptr, raycasting_loop, &bundle);
	mlx_loop(game.mlx_ptr);
	return (EXIT_SUCCESS);
}
