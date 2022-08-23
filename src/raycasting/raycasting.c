/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:55:36 by shaas             #+#    #+#             */
/*   Updated: 2022/08/23 21:26:43 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static inline void	ray_calc(t_raycasting_calc *cast, t_game *game, int ray_iter)
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

static inline void	put_floor_ceiling(mlx_image_t *mlx_img, t_scene_description *scene_desc)
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
	check_movement(&(game->vectors), scene_desc->map_content, game->mlx_ptr);
	check_rotation(&(game->vectors), game->mlx_ptr);
}
