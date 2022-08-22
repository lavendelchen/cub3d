/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_movement_rotation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 16:50:55 by shaas             #+#    #+#             */
/*   Updated: 2022/08/22 16:53:05 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	check_left_right_movement(
	struct s_vectors *vectors, char **map, mlx_t *mlx_ptr)
{
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_D))
	{
		if (map[(int)(vectors->player_position[X] - (vectors->player_direction[Y]
		* MOVESPEED))][(int)vectors->player_position[Y]] != '1')
			vectors->player_position[X]
			-= vectors->player_direction[Y] * MOVESPEED;
		if (map[(int)vectors->player_position[X]]
		[(int)(vectors->player_position[Y] + (vectors->player_direction[X]
		* MOVESPEED))] != '1')
			vectors->player_position[Y]
			+= vectors->player_direction[X] * MOVESPEED;
		print_data(vectors);
	}
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_A))
	{
		if (map[(int)(vectors->player_position[X] + (vectors->player_direction[Y]
		* MOVESPEED))][(int)vectors->player_position[Y]] != '1')
			vectors->player_position[X]
			+= vectors->player_direction[Y] * MOVESPEED;
		if (map[(int)vectors->player_position[X]]
		[(int)(vectors->player_position[Y] - (vectors->player_direction[X]
		* MOVESPEED))] != '1')
			vectors->player_position[Y]
			-= vectors->player_direction[X] * MOVESPEED;
		print_data(vectors);
	}
}

void	check_forward_back_movement(
	struct s_vectors *vectors, char **map, mlx_t *mlx_ptr)
{
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_W))
	{
		if (map[(int)(vectors->player_position[X] + (vectors->player_direction[X]
		* MOVESPEED))][(int)vectors->player_position[Y]] != '1')
			vectors->player_position[X]
			+= vectors->player_direction[X] * MOVESPEED;
		if (map[(int)vectors->player_position[X]]
		[(int)(vectors->player_position[Y] + (vectors->player_direction[Y]
		* MOVESPEED))] != '1')
			vectors->player_position[Y]
			+= vectors->player_direction[Y] * MOVESPEED;
		print_data(vectors);
	}
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_S))
	{
		if (map[(int)(vectors->player_position[X] - (vectors->player_direction[X]
		* MOVESPEED))][(int)vectors->player_position[Y]] != '1')
			vectors->player_position[X]
			-= vectors->player_direction[X] * MOVESPEED;
		if (map[(int)vectors->player_position[X]]
		[(int)(vectors->player_position[Y] - (vectors->player_direction[Y]
		* MOVESPEED))] != '1')
			vectors->player_position[Y]
			-= vectors->player_direction[Y] * MOVESPEED;
		print_data(vectors);
	}
}

void	check_rotation(struct s_vectors *vectors, mlx_t *mlx_ptr)
{
	double	old_player_dir[1];
	double	old_camera_plane[1];

	old_player_dir[X] = vectors->player_direction[X];
	old_camera_plane[X] = vectors->camera_plane[X];
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_LEFT))
	{
		vectors->player_direction[X] = vectors->player_direction[X] * cos(-ROTSPEED)
		- vectors->player_direction[Y] * sin(-ROTSPEED);
		vectors->player_direction[Y] = old_player_dir[X] * sin(-ROTSPEED)
		+ vectors->player_direction[Y] * cos(-ROTSPEED);
		vectors->camera_plane[X] = vectors->camera_plane[X]
		* cos(-ROTSPEED) - vectors->camera_plane[Y] * sin(-ROTSPEED);
		vectors->camera_plane[Y] = old_camera_plane[X] * sin(-ROTSPEED)
		+ vectors->camera_plane[Y] * cos(-ROTSPEED);
		print_data(vectors);
	}
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_RIGHT))
	{
		vectors->player_direction[X] = vectors->player_direction[X] * cos(ROTSPEED)
		- vectors->player_direction[Y] * sin(ROTSPEED);
		vectors->player_direction[Y] = old_player_dir[X] * sin(ROTSPEED)
		+ vectors->player_direction[Y] * cos(ROTSPEED);
		vectors->camera_plane[X] = vectors->camera_plane[X]
		* cos(ROTSPEED) - vectors->camera_plane[Y] * sin(ROTSPEED);
		vectors->camera_plane[Y] = old_camera_plane[X] * sin(ROTSPEED)
		+ vectors->camera_plane[Y] * cos(ROTSPEED);
		print_data(vectors);
	}
}
