/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 16:09:23 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/23 16:10:05 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static inline void	move(
	double new_player_y, double new_player_x, char **map, t_vectors *vectors
	)
{
	if (map[(int)new_player_y][(int)new_player_x] != '1')
	{
		vectors->player_position[Y] = new_player_y;
		vectors->player_position[X] = new_player_x;
	}
}

void	move_left_right(
	t_vectors *vectors, char **map, mlx_t *mlx_ptr)
{
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_D))
	{
		move(vectors->player_position[Y],
			vectors->player_position[X]
			- (vectors->player_direction[Y] * MOVESPEED), map, vectors);
		move(vectors->player_position[Y]
			+ (vectors->player_direction[X] * MOVESPEED),
			vectors->player_position[X], map, vectors);
		print_data(vectors);
	}
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_A))
	{
		move(vectors->player_position[Y],
			vectors->player_position[X]
			+ (vectors->player_direction[Y] * MOVESPEED), map, vectors);
		move(vectors->player_position[Y]
			- (vectors->player_direction[X] * MOVESPEED),
			vectors->player_position[X], map, vectors);
		print_data(vectors);
	}
}

void	move_forward_back(
	struct s_vectors *vectors, char **map, mlx_t *mlx_ptr)
{
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_W))
	{
		move(vectors->player_position[Y],
			vectors->player_position[X]
			+ (vectors->player_direction[X] * MOVESPEED), map, vectors);
		move(
			vectors->player_position[Y]
			+ (vectors->player_direction[Y] * MOVESPEED),
			vectors->player_position[X], map, vectors);
		print_data(vectors);
	}
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_S))
	{
		move(vectors->player_position[Y],
			vectors->player_position[X]
			- (vectors->player_direction[X] * MOVESPEED), map, vectors);
		move(vectors->player_position[Y]
			- (vectors->player_direction[Y] * MOVESPEED),
			vectors->player_position[X], map, vectors);
		print_data(vectors);
	}
}
