/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/21 16:55:00 by tschmitt         ###   ########.fr       */
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

enum e_direction
{
	NO = 0,
	SO = 1,
	WE = 2,
	EA = 3,
};

enum e_grid
{
	X = 0,
	Y = 1
};

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

/**
 * Holds Information about the Position of the Player in the Level
 * @param x Refers to the east / west Position of the Player
 * @param y Refers to the up / down Position of the Player, always 0
 * @param z Refers to the north / south Position of the Player
 */
typedef struct s_player
{
	int					position[2];
	enum e_direction	direction;
}	t_player;

typedef struct s_scene_description
{
	char		*textures[4];
	t_color 	floor_color;
	t_color 	ceiling_color;
	char		**map_content;
	t_player	player;
}	t_scene_description;

typedef struct s_game
{

}	t_game;

int		rgba(int r, int g, int b, int a);
int		parser(const char *scene_description_file_path, t_scene_description *scene_description);

#endif
