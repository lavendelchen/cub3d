/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/21 16:22:18 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void temp_init_scene_description(t_scene_description *scene_description)
{
	scene_description->textures[NO] = ft_strdup("./textures_xpm/naturalbricks_hd_verydark.xpm");
	scene_description->textures[SO] = ft_strdup("./textures_xpm/naturalbricks_hd_middle.xpm");
	scene_description->textures[WE] = ft_strdup("./textures_xpm/naturalbricks_hd_middle.xpm");
	scene_description->textures[EA] = ft_strdup("./textures_xpm/naturalbricks_hd_middle.xpm");

	scene_description->floor_color.r = 1;
	scene_description->floor_color.g = 2;
	scene_description->floor_color.b = 3;
	scene_description->ceiling_color.r = 3;
	scene_description->ceiling_color.g = 2;
	scene_description->ceiling_color.b = 1;

	scene_description->map_content = ft_calloc(27, sizeof(char *));
	scene_description->map_content[0] = ft_strdup("11111111111111111");
	scene_description->map_content[1] = ft_strdup("10000000000000001");
	scene_description->map_content[2] = ft_strdup("10100000100000101");
	scene_description->map_content[3] = ft_strdup("10000000000000001");
	scene_description->map_content[4] = ft_strdup("10001000100010001");
	scene_description->map_content[5] = ft_strdup("10000000000000001");
	scene_description->map_content[6] = ft_strdup("10101010N01010101");
	scene_description->map_content[7] = ft_strdup("10000000000000001");
	scene_description->map_content[8] = ft_strdup("10001000100100001");
	scene_description->map_content[9] = ft_strdup("10000000000000001");
	scene_description->map_content[10] = ft_strdup("10100000100001001");
	scene_description->map_content[11] = ft_strdup("10000000000000001");
	scene_description->map_content[12] = ft_strdup("11111111111111111");
	scene_description->map_content[13] = ft_strdup("11111111111111111");
	scene_description->map_content[14] = ft_strdup("10000000000000001");
	scene_description->map_content[15] = ft_strdup("10100000100000101");
	scene_description->map_content[16] = ft_strdup("10000000000000001");
	scene_description->map_content[17] = ft_strdup("10001000100010001");
	scene_description->map_content[18] = ft_strdup("10000000000000001");
	scene_description->map_content[19] = ft_strdup("10101010001010101");
	scene_description->map_content[20] = ft_strdup("10000000000000001");
	scene_description->map_content[21] = ft_strdup("10001000100100001");
	scene_description->map_content[22] = ft_strdup("10000000000000001");
	scene_description->map_content[23] = ft_strdup("10100000100001001");
	scene_description->map_content[24] = ft_strdup("10000000000000001");
	scene_description->map_content[25] = ft_strdup("11111111111111111");
	
	scene_description->starting_position[X] = 6;
	scene_description->starting_position[Y] = 8;
	scene_description->starting_direction = NO;

	for (int i = 0; i < 4; i++)
		printf("Himmelsrichtung: %s\n", scene_description->textures[i]);
	for (int i = 0; i < 26; i++)
		printf("[%s]\n", scene_description->map_content[i]);
}

bool init_directions_no_so(int starting_direction, struct s_vectors *vectors)
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

void init_directions_we_ea(int starting_direction, struct s_vectors *vectors)
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

void init_game(t_game *game, t_scene_description *scene_description)
{
	game->vectors.player_position[X] = scene_description->starting_position[X] + 0.5; // so that we are in the middle of that square
	game->vectors.player_position[Y] = scene_description->starting_position[Y] + 0.5; // same as above // or minus - ?
	if (!init_directions_no_so(scene_description->starting_direction, &(game->vectors)))
		init_directions_we_ea(scene_description->starting_direction, &(game->vectors));
	game->current_frame_time = 0;
	game->last_frame_time = 0;
}

void	init_wall_hit_calc(t_raycasting_calc *cast, t_game *game)
{
	if (cast->ray_vector[X] < 0)
	{
		cast->direction[X] = -1;
		cast->player_to_tile_border_distance[X] = (game->vectors.player_position[X] - cast->tile[X]) * cast->tile_border_distance[X];
	}
	else
	{
	  cast->direction[X] = 1;
	  cast->player_to_tile_border_distance[X] = (cast->tile[X] + 1.0 - game->vectors.player_position[X]) * cast->tile_border_distance[X];
	}
	if (cast->ray_vector[Y] < 0)
	{
		cast->direction[Y] = -1;
		cast->player_to_tile_border_distance[Y] = (game->vectors.player_position[Y] - cast->tile[Y]) * cast->tile_border_distance[Y];
	}
	else
	{
	  cast->direction[Y] = 1;
	  cast->player_to_tile_border_distance[Y] = (cast->tile[Y] + 1.0 - game->vectors.player_position[Y]) * cast->tile_border_distance[Y];
	}
}

void	wall_hit_calc(t_raycasting_calc *cast, t_scene_description *scene_description)
{
	while (true)
	{
		if (cast->player_to_tile_border_distance[X] < cast->player_to_tile_border_distance[Y])
		{
			cast->player_to_tile_border_distance[X] += cast->tile_border_distance[X];
			cast->tile[X] += cast->direction[X];
			cast->hit_border = NO_SO; //correct?
		}
		else
		{
			cast->player_to_tile_border_distance[Y] += cast->tile_border_distance[Y];
			cast->tile[Y] += cast->direction[Y];
			cast->hit_border = WE_EA; //correct?
		}
		if (scene_description->map_content[cast->tile[Y]][cast->tile[X]] == '1')
			return ;
	}
}

void	draw_wall(t_raycasting_calc *cast, t_game *game, t_scene_description *scene_description, int ray_iter)
{
	int	wall_height;
	int	first_pixel;
	int	last_pixel;

	wall_height = (int)((SCREENHEIGHT * WALLHEIGHT) / cast->result_wall_distance);
	first_pixel = (SCREENHEIGHT / 2) - (wall_height / 2);
	if (first_pixel < 0)
		first_pixel = 0;
	last_pixel = (SCREENHEIGHT / 2) + (wall_height / 2);
	if (last_pixel >= SCREENHEIGHT)
		last_pixel = SCREENHEIGHT - 1;
	//later textures, now just some color
	printf("\nITERATION %d\n", ray_iter);
	printf("Wall height: %d\n", wall_height);
	printf("First pixel: %d\n", first_pixel);
	printf("Last pixel: %d\n", last_pixel);
	while (first_pixel >= last_pixel)
	{
		mlx_put_pixel(game->mlx_img, ray_iter, first_pixel, 0xFF0000FF);
		first_pixel++;
	}
	(void)scene_description;
}

void	raycasting_loop(t_game *game, t_scene_description *scene_description)
{
	int					ray_iter;
	t_raycasting_calc	cast;

	while (true)
	{
		ray_iter = 0;
		while (ray_iter < SCREENWIDTH)
		{
			cast.camera_plane_part = (ray_iter * 2 / SCREENWIDTH) - 1;
			cast.ray_vector[X] = game->vectors.player_direction[X] + (game->vectors.camera_plane[X] * cast.camera_plane_part);
			cast.ray_vector[Y] = game->vectors.player_direction[Y] + (game->vectors.camera_plane[Y] * cast.camera_plane_part);
			cast.tile[X] = (int)game->vectors.player_position[X];
			cast.tile[Y] = (int)game->vectors.player_position[Y];
			cast.tile_border_distance[X]
			= (cast.ray_vector[X] != 0) ? fabs(1 / cast.ray_vector[X]) : INFINITY; // let's see if infinity works
			cast.tile_border_distance[Y]
			= (cast.ray_vector[Y] != 0) ? fabs(1 / cast.ray_vector[Y]) : INFINITY; // let's see if infinity works
			init_wall_hit_calc(&cast, game);
			wall_hit_calc(&cast, scene_description);
			if (cast.hit_border == NO_SO)
				cast.result_wall_distance = cast.player_to_tile_border_distance[X] - cast.tile_border_distance[X];
			else
				cast.result_wall_distance = cast.player_to_tile_border_distance[Y] - cast.tile_border_distance[Y];
			draw_wall(&cast, game, scene_description, ray_iter);
			ray_iter++;
		}
		return ; // testing
	}
}

int	main(int argc, const char *argv[])
{
	t_scene_description	scene_description;
	temp_init_scene_description(&scene_description); //
	t_game				game;

	(void)argc; //
	(void)argv; //
	//if (argc != 2)
	//{
	//	ft_putstr_fd("Error\nOne Argument Needed\n", STDERR_FILENO);
	//	return (EXIT_FAILURE);
	//}
	//if (parser(argv[1], &scene_description) != EXIT_SUCCESS)
	//	return (EXIT_FAILURE);

	init_game(&game, &scene_description);
	game.mlx_ptr = mlx_init(SCREENWIDTH, SCREENHEIGHT, "🌈RainbowCube🌈", true); //later resize false?
	game.mlx_img = mlx_new_image(game.mlx_ptr, SCREENWIDTH, SCREENHEIGHT);
	mlx_image_to_window(game.mlx_ptr, game.mlx_img, 0, 0);
	printf("INFINITY: %f\n", INFINITY);
	raycasting_loop(&game, &scene_description);
	while (true);
	return (EXIT_SUCCESS);
}
