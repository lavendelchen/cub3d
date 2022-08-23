/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 20:09:31 by shaas             #+#    #+#             */
/*   Updated: 2022/08/23 21:46:00 by shaas            ###   ########.fr       */
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

/* 
player_position gets added 0.5 so we don't start at the corner
of the starting tile, but the middle of it
*/
void	init_game(t_game *game, t_scene_description *scene_desc)
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
}
