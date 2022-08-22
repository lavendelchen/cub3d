/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 16:53:18 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/22 16:53:19 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	close_at_esc(mlx_key_data_t key_data, void *arg)
{
	t_bundle	*bundle;

	bundle = (t_bundle *)arg;
	if (key_data.key == MLX_KEY_ESCAPE)
		mlx_close_window(bundle->game->mlx_ptr);
}

void	free_at_window_close(void *arg)
{
	t_bundle	*bundle;

	bundle = (t_bundle *)arg;
	ft_free_split(bundle->scene_desc->map_content);
	mlx_delete_image(bundle->game->mlx_ptr, bundle->game->mlx_img);
}
