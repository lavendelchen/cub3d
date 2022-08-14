/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/02 14:48:22 by tschmitt         ###   ########.fr       */
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
		vectors->player_direction[Y] = 1;
		vectors->camera_plane[X] = FOV;
		vectors->camera_plane[Y] = 0;
		return (true);
	}
	else if (starting_direction == SO)
	{
		vectors->player_direction[X] = 0;
		vectors->player_direction[Y] = -1;
		vectors->camera_plane[X] = FOV*-1;
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
		vectors->camera_plane[Y] = FOV;
	}
	else if (starting_direction == EA)
	{
		vectors->player_direction[X] = 1;
		vectors->player_direction[Y] = 0;
		vectors->camera_plane[X] = 0;
		vectors->camera_plane[Y] = FOV*-1;
	}
}

void init_game(t_game *game, t_scene_description *scene_description)
{
	game->vectors.player_position[X] = scene_description->starting_position[X] + 0.5; // so that we are in the middle of that square
	game->vectors.player_position[Y] = scene_description->starting_position[Y] + 0.5; // same as above
	if (!init_directions_no_so(scene_description->starting_direction, &(game->vectors)))
		init_directions_we_ea(scene_description->starting_direction, &(game->vectors));
	game->current_frame_time = 0;
	game->last_frame_time = 0;
}

void	raycasting_loop(t_game *game, t_scene_description *scene_description)
{
	int					screen_x_iter;
	t_raycasting_calc	calc;

	while (true)
	{
		screen_x_iter = 0;
		while (screen_x_iter < SCREENWIDTH)
		{
			calc.camera_plane_part = (screen_x_iter * 2 / SCREENWIDTH) - 1;
			calc.ray_vector[X] = player_direction[X] + (camera_plane[X] * camera_plane_part);
			calc.ray_vector[Y] = player_direction[Y] + (camera_plane[Y] * camera_plane_part);
			calc.player_tile[X] = (int)game->vectors.player_position[X];
			calc.player_tile[Y] = (int)game->vectors.player_position[Y];


			screen_x_iter++;
		}
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
	raycasting_loop(&game, &scene_description);
	return (EXIT_SUCCESS);
}
