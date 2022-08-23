/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 16:10:46 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/23 20:42:31 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static inline void	rotate_left(
	t_vectors *vectors,
	const double *old_player_dir, const double *old_camera_plane
	)
{
	vectors->player_direction[X] = vectors->player_direction[X] \
	* cos(-ROTSPEED) - vectors->player_direction[Y] * sin(-ROTSPEED);
	vectors->player_direction[Y] = old_player_dir[X] * sin(-ROTSPEED) \
	+ vectors->player_direction[Y] * cos(-ROTSPEED);
	vectors->camera_plane[X] = vectors->camera_plane[X] \
	* cos(-ROTSPEED) - vectors->camera_plane[Y] * sin(-ROTSPEED);
	vectors->camera_plane[Y] = old_camera_plane[X] * sin(-ROTSPEED) \
	+ vectors->camera_plane[Y] * cos(-ROTSPEED);
	print_data(vectors);
}

static inline void	rotate_right(
	t_vectors *vectors,
	const double *old_player_dir, const double *old_camera_plane
	)
{
	vectors->player_direction[X] = vectors->player_direction[X] * cos(ROTSPEED) \
	- vectors->player_direction[Y] * sin(ROTSPEED);
	vectors->player_direction[Y] = old_player_dir[X] * sin(ROTSPEED) \
	+ vectors->player_direction[Y] * cos(ROTSPEED);
	vectors->camera_plane[X] = vectors->camera_plane[X] \
	* cos(ROTSPEED) - vectors->camera_plane[Y] * sin(ROTSPEED);
	vectors->camera_plane[Y] = old_camera_plane[X] * sin(ROTSPEED) \
	+ vectors->camera_plane[Y] * cos(ROTSPEED);
	print_data(vectors);
}

void	check_rotation(struct s_vectors *vectors, mlx_t *mlx_ptr)
{
	double	old_player_dir[1];
	double	old_camera_plane[1];

	old_player_dir[X] = vectors->player_direction[X];
	old_camera_plane[X] = vectors->camera_plane[X];
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_LEFT))
		rotate_left(vectors, old_player_dir, old_camera_plane);
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_RIGHT))
		rotate_right(vectors, old_player_dir, old_camera_plane);
}
