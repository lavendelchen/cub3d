/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/22 20:09:57 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	init_wall_hit_calc(t_raycasting_calc *cast, t_game *game)
{
	if (cast->ray_vector[X] < 0)
	{
		cast->direction[X] = -1;
		cast->player_to_tile_border[X] = (game->vectors.player_position[X]
				- cast->tile[X]) * cast->tile_border_distance[X];
		cast->potential_wall[X] = WE;
	}
	else
	{
		cast->direction[X] = 1;
		cast->player_to_tile_border[X] = (cast->tile[X] + 1.0 \
			- game->vectors.player_position[X]) * cast->tile_border_distance[X];
		cast->potential_wall[X] = EA;
	}
	if (cast->ray_vector[Y] < 0)
	{
		cast->direction[Y] = -1;
		cast->player_to_tile_border[Y] = (game->vectors.player_position[Y]
				- cast->tile[Y]) * cast->tile_border_distance[Y];
		cast->potential_wall[Y] = NO;
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

void	draw_wall(t_raycasting_calc *cast, t_game *game,
					t_scene_description *scene_desc, int ray_iter)
{
	int	wall_height;
	int	first_pixel;
	int	last_pixel;

	wall_height = (int)(SCREENHEIGHT * WALLHEIGHT) / cast->result_wall_distance;
	first_pixel = (SCREENHEIGHT * WALLHEIGHT / 2) - (wall_height / 2);
	if (first_pixel < 0)
		first_pixel = 0;
	last_pixel = (SCREENHEIGHT * WALLHEIGHT / 2) + (wall_height / 2);
	if (last_pixel >= (SCREENHEIGHT * WALLHEIGHT))
		last_pixel = SCREENHEIGHT * WALLHEIGHT - 1;
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
	cast->camera_plane_part = (ray_iter * 2 / (double)SCREENWIDTH) - 1;
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

void	raycasting_loop(void *bundle)
{
	static int					ray_iter;	// x
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
	paste_png(game, "textures/pride_textures/bisexual_pride_flag.png");
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
	mlx_close_hook(game.mlx_ptr, free_at_window_close, &bundle);
	mlx_key_hook(game.mlx_ptr, close_at_esc, &bundle);
	mlx_loop_hook(game.mlx_ptr, raycasting_loop, &bundle);
	mlx_loop(game.mlx_ptr);
	mlx_terminate(game.mlx_ptr);
	return (EXIT_SUCCESS);
}
