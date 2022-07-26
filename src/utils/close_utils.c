/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 16:53:18 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/24 01:20:34 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	free_at_window_close(void *arg)
{
	t_bundle	*bundle;

	bundle = (t_bundle *)arg;
	ft_free_split(bundle->scene_desc->map_content);
	free(bundle->scene_desc->textures[0]);
	free(bundle->scene_desc->textures[1]);
	free(bundle->scene_desc->textures[2]);
	free(bundle->scene_desc->textures[3]);
	mlx_delete_image(bundle->game->mlx_ptr, bundle->game->mlx_img);
	mlx_delete_texture(bundle->game->wall[NO]);
	mlx_delete_texture(bundle->game->wall[SO]);
	mlx_delete_texture(bundle->game->wall[WE]);
	mlx_delete_texture(bundle->game->wall[EA]);
}

void	close_at_esc(mlx_key_data_t key_data, void *arg)
{
	t_bundle	*bundle;

	bundle = (t_bundle *)arg;
	if (key_data.key == MLX_KEY_ESCAPE)
	{
		free_at_window_close(arg);
		mlx_close_window(bundle->game->mlx_ptr);
	}
}

int	put_error_msg(char *msg)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	return (EXIT_FAILURE);
}
