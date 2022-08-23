/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/24 01:25:46 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

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
	if (init_game(&game, &scene_desc) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	mlx_key_hook(game.mlx_ptr, close_at_esc, &bundle);
	mlx_close_hook(game.mlx_ptr, free_at_window_close, &bundle);
	if (!mlx_loop_hook(game.mlx_ptr, raycasting_loop, &bundle))
		return (put_error_msg("Error\nMlx hook couldn't be added\n"));
	mlx_loop(game.mlx_ptr);
	mlx_terminate(game.mlx_ptr);
	if (MUSIC)
		kill_music();
	return (EXIT_SUCCESS);
}
