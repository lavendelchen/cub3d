/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 20:09:31 by shaas             #+#    #+#             */
/*   Updated: 2022/08/23 16:12:56 by tschmitt         ###   ########.fr       */
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
	game->wall[NO] = mlx_load_png(scene_desc->textures[NO]);
	game->wall[SO] = mlx_load_png(scene_desc->textures[SO]);
	game->wall[WE] = mlx_load_png(scene_desc->textures[WE]);
	game->wall[EA] = mlx_load_png(scene_desc->textures[EA]);
}
