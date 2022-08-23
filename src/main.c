/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/23 16:09:01 by tschmitt         ###   ########.fr       */
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
		cast->potential_wall_direction[X] = WE;
	}
	else
	{
		cast->direction[X] = 1;
		cast->player_to_tile_border[X] = (cast->tile[X] + 1.0 \
			- game->vectors.player_position[X]) * cast->tile_border_distance[X];
		cast->potential_wall_direction[X] = EA;
	}
	if (cast->ray_vector[Y] < 0)
	{
		cast->direction[Y] = -1;
		cast->player_to_tile_border[Y] = (game->vectors.player_position[Y]
				- cast->tile[Y]) * cast->tile_border_distance[Y];
		cast->potential_wall_direction[Y] = NO;
	}
	else
	{
		cast->direction[Y] = 1;
		cast->player_to_tile_border[Y] = (cast->tile[Y] + 1.0 \
			- game->vectors.player_position[Y]) * cast->tile_border_distance[Y];
		cast->potential_wall_direction[Y] = SO;
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
	{
		cast->wall_distance = cast->player_to_tile_border[X]
			- cast->tile_border_distance[X];
		cast->wall_direction = cast->potential_wall_direction[X];
	}
	else
	{
		cast->wall_distance = cast->player_to_tile_border[Y]
			- cast->tile_border_distance[Y];
		cast->wall_direction = cast->potential_wall_direction[Y];
	}
}

int	tex_pixel_color(t_texture_calc *tex, mlx_texture_t *wall)
{
	int	pixel;

	pixel = tex->texture_pixel[X] * tex->texture_pixel[Y] * 4;
	return (rgba(wall->pixels[pixel], wall->pixels[pixel + 1],
				wall->pixels[pixel + 2], wall->pixels[pixel + 3]));
}

void	draw_wall(t_raycasting_calc *cast, t_game *game, int ray_iter)
{
	t_texture_calc	tex;

	tex.wall_height = (int)(SCREENHEIGHT * WALLHEIGHT) / cast->wall_distance;
	tex.first_pixel = (SCREENHEIGHT * WALLHEIGHT / 2) - (tex.wall_height / 2);
	if (tex.first_pixel < 0)
		tex.first_pixel = 0;
	tex.last_pixel = (SCREENHEIGHT * WALLHEIGHT / 2) + (tex.wall_height / 2);
	if (tex.last_pixel >= (SCREENHEIGHT * WALLHEIGHT))
		tex.last_pixel = SCREENHEIGHT * WALLHEIGHT - 1;
	if (cast->hit_border == NO_SO)
		tex.wall_hitpoint = game->vectors.player_position[Y]
		+ (cast->wall_distance * cast->ray_vector[Y]);
	else
		tex.wall_hitpoint = game->vectors.player_position[X]
		+ (cast->wall_distance * cast->ray_vector[X]);
	tex.wall_hitpoint -= floor(tex.wall_hitpoint);
	tex.texture_pixel[X] = (int)(tex.wall_hitpoint * (double)game->wall[cast->wall_direction]->width);
	if ((cast->hit_border == NO_SO && cast->ray_vector[X] > 0)
		|| (cast->hit_border == WE_EA && cast->ray_vector[Y] < 0))
		tex.texture_pixel[X] = game->wall[cast->wall_direction]->width - tex.texture_pixel[X] - 1;
	tex.step = (double)game->wall[cast->wall_direction]->height / tex.wall_height;
	tex.texture_position = 0;;
	while (tex.first_pixel < tex.last_pixel)
	{
		tex.texture_pixel[Y] = (int)tex.texture_position;
		mlx_put_pixel(game->mlx_img, ray_iter, tex.first_pixel, tex_pixel_color(&tex, game->wall[cast->wall_direction]));
		tex.texture_position += tex.step;
		tex.first_pixel++;
	}
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
		draw_wall(&cast, game, ray_iter);
		ray_iter++;
	}
	move_forward_back(&(game->vectors), scene_desc->map_content, game->mlx_ptr);
	move_left_right(&(game->vectors), scene_desc->map_content, game->mlx_ptr);
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
	mlx_close_hook(game.mlx_ptr, free_at_window_close, &bundle);
	mlx_key_hook(game.mlx_ptr, close_at_esc, &bundle);
	mlx_loop_hook(game.mlx_ptr, raycasting_loop, &bundle);
	mlx_loop(game.mlx_ptr);
	mlx_terminate(game.mlx_ptr);
	return (EXIT_SUCCESS);
}
