/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/07/11 17:33:31 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

#define WIDTH 256
#define HEIGHT 256

mlx_image_t	*g_img;

void	hook(void *param)
{
    if (mlx_is_key_down(param, MLX_KEY_ESCAPE))
        mlx_close_window(param);
    if (mlx_is_key_down(param, MLX_KEY_UP))
        g_img->instances[0].y -= 5;
    if (mlx_is_key_down(param, MLX_KEY_DOWN))
        g_img->instances[0].y += 5;
    if (mlx_is_key_down(param, MLX_KEY_LEFT))
        g_img->instances[0].x -= 5;
    if (mlx_is_key_down(param, MLX_KEY_RIGHT))
        g_img->instances[0].x += 5;
}

int32_t	main(void)
{
    mlx_t	*mlx;

    mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true);
    if (!mlx)
        exit(EXIT_FAILURE);
    g_img = mlx_new_image(mlx, 128, 128);
    memset(g_img->pixels, 255, g_img->width * g_img->height * sizeof(int));
    mlx_image_to_window(mlx, g_img, 0, 0);
    mlx_loop_hook(mlx, &hook, mlx);
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return (EXIT_SUCCESS);
}
