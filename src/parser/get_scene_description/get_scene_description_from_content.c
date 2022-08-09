/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_scene_description_from_content.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 17:49:01 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/08 22:46:44 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"
#include "../../../inc/parser.h"

int	set_ceiling_color(
		t_scene_description *scene_description,
		int r, int g, int b
)
{
	static bool	has_been_set = false;

	if (has_been_set)
		return (print_error_return(INVALID_IDENTIFIER_ERROR, 1));
	if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255))
		return (print_error_return(INVALID_COLOR_ERROR, 1));
	scene_description->ceiling_color.r = r;
	scene_description->ceiling_color.g = g;
	scene_description->ceiling_color.b = b;
	has_been_set = true;
	return (EXIT_SUCCESS);
}

int	set_floor_color(
		t_scene_description *scene_description,
		int r, int g, int b
		)
{
	static bool	has_been_set = false;

	if (has_been_set)
		return (print_error_return(INVALID_IDENTIFIER_ERROR, 1));
	if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255))
		return (print_error_return(INVALID_COLOR_ERROR, 1));
	scene_description->floor_color.r = r;
	scene_description->floor_color.g = g;
	scene_description->floor_color.b = b;
	has_been_set = true;
	return (EXIT_SUCCESS);
}

bool	is_valid_color_description(char *desc)
{
	int	i;

	if (ft_chrcount(desc, ',') != 2)
		return (false);
	i = 0;
	while (desc[i] != '\0')
	{
		if (desc[i] == ',')
		{
			if (i == 0 || desc[i + 1] == '\0')
				return (false);
			if (!ft_isdigit(desc[i - 1]) && !ft_isalnum(desc[i + 1]))
				return (false);
		}
		else if (!ft_isdigit(desc[i]))
			return (false);
		i += 1;
	}
	return (true);
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
