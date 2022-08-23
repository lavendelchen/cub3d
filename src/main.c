/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/23 19:57:30 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

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
