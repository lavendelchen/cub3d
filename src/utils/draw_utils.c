/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 19:57:21 by shaas             #+#    #+#             */
/*   Updated: 2022/08/22 21:04:35 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	put_square(t_square_data *square)
{
	int	iter[2];

	iter[Y] = 0;
	while (iter[Y] < square->length[Y]
		&& iter[Y] + square->start_pixel[Y] < (int)square->mlx_img->height)
	{
		iter[X] = 0;
		while (iter[X] < square->length[X]
			&& iter[X] + square->start_pixel[X] < (int)square->mlx_img->width)
		{
			mlx_put_pixel(square->mlx_img, square->start_pixel[X] + iter[X],
				square->start_pixel[Y] + iter[Y], square->color);
			iter[X]++;
		}
		iter[Y]++;
	}
}


void	paste_png(t_game *game, char *png)
{
	mlx_texture_t*	tex = mlx_load_png(png);
	//mlx_image_t*	img = mlx_texture_to_image(game->mlx_ptr, tex);

	int	image[2];
	int	iter;
	image[Y] = 0;
	iter = 0;
	while (image[Y] < (int)tex->height)
	{
		image[X] = 0;
		while (image[X] < (int)tex->width)
		{
			mlx_put_pixel(game->mlx_img, image[X] + 300, image[Y] + 300, rgba(tex->pixels[iter], tex->pixels[iter + 1], tex->pixels[iter + 2], tex->pixels[iter + 3]));
			iter += 4;
			image[X]++;
		}
		image[Y]++;
	}
	mlx_delete_texture(tex);
}

void	paste_texture(mlx_texture_t *tex, t_game *game)
{
	int	image[2];
	int	iter;
	image[Y] = 0;
	iter = 0;
	while (image[Y] < (int)tex->height)
	{
		image[X] = 0;
		while (image[X] < (int)tex->width)
		{
			mlx_put_pixel(game->mlx_img, image[X] + 300, image[Y] + 300, rgba(tex->pixels[iter], tex->pixels[iter + 1], tex->pixels[iter + 2], tex->pixels[iter + 3]));
			iter += 4;
			image[X]++;
		}
		image[Y]++;
	}
}
