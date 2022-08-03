/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/02 13:34:59 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/* mlx library */
# include "../lib/MLX42/include/MLX42/MLX42.h"

/* Libft Library */
# include "../lib/libft/include/libft.h"

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <memory.h>


/* RGBA / UTILS */
int     rgba(int r, int g, int b, int a);

/* Parser */

enum e_texture_iterator
{
	NO = 0,
	SO = 1,
	WE = 2,
	EA = 3,
};

typedef struct s_scene_description
{
	char	*textures[4];
	struct s_color
	{
		int			r;
		int			g;
		int			b;
	}	floor_color;
	struct s_color	celling_color;
	char			**map_content;
}	t_scene_description;

int 	parser(const char *scene_description_file_path, t_scene_description *scene_description);

#endif
