/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 20:09:31 by shaas             #+#    #+#             */
/*   Updated: 2022/08/24 00:37:04 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static inline bool	init_directions_no_so(
	int starting_direction, t_vectors *vectors
	)
{
	if (starting_direction == NO)
	{
		vectors->player_direction[X] = 0;
		vectors->player_direction[Y] = -1;
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

static inline void	init_directions_we_ea(
	int starting_direction, t_vectors *vectors
	)
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

static inline void	init_music(char *start_texture)
{
	if (!MUSIC)
		return ;
	if (ft_strstr(start_texture, "flag"))
		system("afplay ./music/nyan-cat.mp3 &");
	else if (ft_strstr(start_texture, "arcade"))
		system("afplay ./music/fun-disco.mp3 &");
	else if (ft_strstr(start_texture, "cloud"))
		system("afplay ./music/peaceful-garden.mp3 &");
	else if (ft_strstr(start_texture, "ground"))
		system("afplay ./music/droplets-in-a-cave.mp3 &");
}

/*
player_position gets added 0.5 so we don't start at the corner
of the starting tile, but the middle of it
*/
int	init_game(t_game *game, t_scene_description *scene_desc)
{
	game->vectors.player_position[X]
		= scene_desc->player.position[X] + 0.5;
	game->vectors.player_position[Y]
		= scene_desc->player.position[Y] + 0.5;
	if (!init_directions_no_so(
			scene_desc->player.direction, &(game->vectors)))
		init_directions_we_ea(
			scene_desc->player.direction, &(game->vectors));
	game->wall[NO] = mlx_load_png(scene_desc->textures[NO]);
	game->wall[SO] = mlx_load_png(scene_desc->textures[SO]);
	game->wall[WE] = mlx_load_png(scene_desc->textures[WE]);
	game->wall[EA] = mlx_load_png(scene_desc->textures[EA]);
	if (!game->wall[NO] || !game->wall[SO]
		|| !game->wall[WE] || !game->wall[EA])
		return (put_error_msg("Error\nPNG could not be loaded\n"));
	game->mlx_ptr = mlx_init(SCREENWIDTH, SCREENHEIGHT, "🌈RainbowCube🌈", false);
	if (!game->mlx_ptr)
		return (put_error_msg("Error\nMlx couldn't be initiated\n"));
	game->mlx_img = mlx_new_image(game->mlx_ptr, SCREENWIDTH, SCREENHEIGHT);
	if (!game->mlx_img)
		return (put_error_msg("Error\nMlx image couldn't be allocated\n"));
	if (mlx_image_to_window(game->mlx_ptr, game->mlx_img, 0, 0) == -1)
		return (put_error_msg("Error\nImage couldn't be put to window\n"));
	init_music(scene_desc->textures[scene_desc->player.direction]);
	return (EXIT_SUCCESS);
}
