/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:03:53 by shaas             #+#    #+#             */
/*   Updated: 2022/08/23 21:26:49 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static int	tex_rgba(t_texture_calc *tex, mlx_texture_t *wall)
{
	int	pixel;

	pixel = (tex->texture_pixel[Y] * wall->width + tex->texture_pixel[X]) * 4;
	return (rgba(wall->pixels[pixel], wall->pixels[pixel + 1],
				wall->pixels[pixel + 2], wall->pixels[pixel + 3]));
}

static inline void	texture_calc(t_texture_calc *tex, t_raycasting_calc *cast, t_game *game)
{
	tex->wall_height = (int)(SCREENHEIGHT * WALLHEIGHT) / cast->wall_distance;
	tex->first_pixel = (SCREENHEIGHT * WALLHEIGHT / 2) - (tex->wall_height / 2);
	if (tex->first_pixel < 0)
		tex->first_pixel = 0;
	tex->last_pixel = (SCREENHEIGHT * WALLHEIGHT / 2) + (tex->wall_height / 2);
	if (tex->last_pixel >= (SCREENHEIGHT * WALLHEIGHT))
		tex->last_pixel = SCREENHEIGHT * WALLHEIGHT - 1;
	if (cast->hit_border == NO_SO)
		tex->wall_hitpoint = game->vectors.player_position[Y]
		+ (cast->wall_distance * cast->ray_vector[Y]);
	else
		tex->wall_hitpoint = game->vectors.player_position[X]
		+ (cast->wall_distance * cast->ray_vector[X]);
	tex->wall_hitpoint -= floor(tex->wall_hitpoint);
	tex->texture_pixel[X] = (int)(tex->wall_hitpoint * (double)game->wall[cast->wall_direction]->width);
	if ((cast->hit_border == NO_SO && cast->ray_vector[X] > 0)
		|| (cast->hit_border == WE_EA && cast->ray_vector[Y] < 0))
		tex->texture_pixel[X] = game->wall[cast->wall_direction]->width - tex->texture_pixel[X] - 1;
	tex->step = ((double)game->wall[cast->wall_direction]->height) / tex->wall_height;
	tex->texture_position = (tex->first_pixel - (SCREENHEIGHT * WALLHEIGHT / 2) + (tex->wall_height / 2)) * tex->step;
}

void	draw_wall(t_raycasting_calc *cast, t_game *game, int ray_iter)
{
	t_texture_calc	tex;

	texture_calc(&tex, cast, game);
	while (tex.first_pixel < tex.last_pixel)
	{
		tex.texture_pixel[Y] = (int)tex.texture_position;
		mlx_put_pixel(game->mlx_img, ray_iter, tex.first_pixel, tex_rgba(&tex, game->wall[cast->wall_direction]));
		tex.texture_position += tex.step;
		tex.first_pixel++;
	}
}
