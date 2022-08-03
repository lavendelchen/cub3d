/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_scene_description_from_content.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 17:49:01 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/03 14:59:57 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include "../../inc/parser.h"

static bool	is_color(char *identifier)
{
	if (ft_strlen(identifier) != 1)
		return (false);
	if (identifier[0] != 'C' && identifier[0] != 'F')
		return (false);
	return (true);
}

static bool	is_texture(char *identifier)
{
	if (ft_strlen(identifier) != 2)
		return (false);
	if (ft_strcmp(identifier, "NO") != 0 && ft_strcmp(identifier, "SO") != 0
		&& ft_strcmp(identifier, "WE") != 0 && ft_strcmp(identifier, "EA") != 0)
		return (false);
	return (true);
}

static int	set_texture_from_content(
		char *identifier,
		char *path,
		t_scene_description *scene_description
		)
{
	if (ft_strcmp(identifier, "NO") == 0)
		scene_description->textures[NO] = ft_strdup(path);
	else if (ft_strcmp(identifier, "SO") == 0)
		scene_description->textures[SO] = ft_strdup(path);
	else if (ft_strcmp(identifier, "WE") == 0)
		scene_description->textures[WE] = ft_strdup(path);
	else if (ft_strcmp(identifier, "EA") == 0)
		scene_description->textures[EA] = ft_strdup(path);
	else
		return (print_error_return(INVALID_IDENTIFIER_ERROR, 1));
	return (EXIT_SUCCESS);
}

static int	set_celling_color(
		t_scene_description *scene_description,
		int r, int g, int b
)
{
	if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255))
		return (print_error_return(INVALID_COLOR_ERROR, 1));
	scene_description->celling_color.r = r;
	scene_description->celling_color.g = g;
	scene_description->celling_color.b = b;
	return (EXIT_SUCCESS);
}

static int	set_floor_color(
		t_scene_description *scene_description,
		int r, int g, int b
		)
{
	if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255))
		return (print_error_return(INVALID_COLOR_ERROR, 1));
	scene_description->floor_color.r = r;
	scene_description->floor_color.g = g;
	scene_description->floor_color.b = b;
	return (EXIT_SUCCESS);
}

static int	set_color_from_content(
		char *identifier,
		char *rgb_color,
		t_scene_description *scene_description
		)
{
	char	**rgb_color_splitted;
	int 	ret;

	ret = -1;
	rgb_color_splitted = ft_split_set(rgb_color, " \t\n\v\f\r,");
	if (rgb_color_splitted == NULL)
		return (print_error_return(ALLOC_ERROR, 1));
	if (ft_strcmp(identifier, "F") == 0)
	{
		ret = set_floor_color(
				scene_description,
				ft_atoi(rgb_color_splitted[0]),
				ft_atoi(rgb_color_splitted[1]),
				ft_atoi(rgb_color_splitted[2]));
	}
	if (ft_strcmp(identifier, "C") == 0)
	{
		ret = set_celling_color(
				scene_description,
				ft_atoi(rgb_color_splitted[0]),
				ft_atoi(rgb_color_splitted[1]),
				ft_atoi(rgb_color_splitted[2]));
	}
	if (ret != EXIT_SUCCESS)
	{
		return (ret);
	}
	ft_free_split(rgb_color_splitted);
	return (EXIT_SUCCESS);
}

static int set_textures_and_colors_from_content(
		char ***scene_file_content_ref,
		t_scene_description *scene_description
		)
{
	char	**line_splitted;
	int 	i;

	i = 0;
	while (*scene_file_content_ref && (*scene_file_content_ref)[i] != NULL && i < 6)
	{
		line_splitted = ft_split_set((*scene_file_content_ref)[i], " \t\n\v\f\r");
		if (line_splitted == NULL)
			return (print_error_return(ALLOC_ERROR, 1));
		if (is_texture(line_splitted[0]))
			set_texture_from_content(line_splitted[0], line_splitted[1], scene_description);
		else if (is_color(line_splitted[0]))
			set_color_from_content(line_splitted[0], line_splitted[1], scene_description);
		else
		{
			ft_free_split(line_splitted);
			return (print_error_return(INVALID_IDENTIFIER_ERROR, 1));
		}
		ft_free_split(line_splitted);
		i += 1;
	}
	*scene_file_content_ref += i;
	return (EXIT_SUCCESS);
}

static int set_map_from_content(
		char **scene_file_content_ref,
		t_scene_description *scene_description
		)
{
	int scene_file_content_len;
	int i;

	scene_file_content_len = ft_str_arr_len(scene_file_content_ref);
	scene_description->map_content = ft_calloc(scene_file_content_len + 1, sizeof(*scene_description->map_content));
	if (scene_description->map_content == NULL)
		return (print_error_return(ALLOC_ERROR, 1));
	i = 0;
	while (scene_file_content_ref[i] != NULL)
	{
		scene_description->map_content[i] = ft_strdup(scene_file_content_ref[i]);
		if (scene_description->map_content[i] == NULL)
			return (print_error_return(ALLOC_ERROR, 1));
		i += 1;
	}
	return (EXIT_SUCCESS);
}

int	get_scene_description_from_content(
		char **scene_file_content,
		t_scene_description *scene_description
)
{
	if (set_textures_and_colors_from_content(
			&scene_file_content, scene_description
		) != 0)
		return (print_error_return("Error\nInvalid Texture / Color\n", 1));
	if (set_map_from_content(
			scene_file_content, scene_description
		) != 0)
		return (print_error_return(INVALID_MAP_ERROR, 1));
	return (EXIT_SUCCESS);
}
