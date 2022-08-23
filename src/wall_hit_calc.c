/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_hit_calc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:54:34 by shaas             #+#    #+#             */
/*   Updated: 2022/08/23 20:56:11 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	wall_hit_calc_result(t_raycasting_calc *cast)
{
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

void	wall_hit_calc(t_raycasting_calc *cast, t_scene_description *scene_desc)
{
	while (true)
	{
		if (cast->player_to_tile_border[X] < cast->player_to_tile_border[Y])
		{
			cast->player_to_tile_border[X] += cast->tile_border_distance[X];
			cast->tile[X] += cast->direction[X];
			cast->hit_border = NO_SO;
		}
		else
		{
			cast->player_to_tile_border[Y] += cast->tile_border_distance[Y];
			cast->tile[Y] += cast->direction[Y];
			cast->hit_border = WE_EA;
		}
		if (scene_desc->map_content[cast->tile[Y]][cast->tile[X]] == '1')
			break ;
	}
	wall_hit_calc_result(cast);
}

static inline void	x_direction(t_raycasting_calc *cast, t_game *game)
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
}

static inline void	y_direction(t_raycasting_calc *cast, t_game *game)
{
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

void	init_wall_hit_calc(t_raycasting_calc *cast, t_game *game)
{
	x_direction(cast, game);
	y_direction(cast, game);
}
