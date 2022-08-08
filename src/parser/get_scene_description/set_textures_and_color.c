/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_textures_and_color.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 22:44:09 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/08 22:46:29 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"
#include "../../../inc/parser.h"

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

static int	set_color(
		char *identifier,
		char *rbg,
		t_scene_description *scene_description
		)
{
	char	**rbg_splitted;
	int		ret;

	ret = -1;
	if (!is_valid_color_description(rbg))
		return (EXIT_FAILURE);
	rbg_splitted = ft_split_set(rbg, " \t\n\v\f\r,");
	if (rbg_splitted == NULL)
		return (print_error_return(ALLOC_ERROR, 1));
	if (ft_strcmp(identifier, "F") == 0)
		ret = set_floor_color(
				scene_description,
				ft_atoi(rbg_splitted[0]),
				ft_atoi(rbg_splitted[1]),
				ft_atoi(rbg_splitted[2])
				);
	if (ft_strcmp(identifier, "C") == 0)
		ret = set_celling_color(
				scene_description,
				ft_atoi(rbg_splitted[0]),
				ft_atoi(rbg_splitted[1]),
				ft_atoi(rbg_splitted[2])
				);
	ft_free_split(rbg_splitted);
	return (ret);
}

static int	set_texture(
		char *identifier,
		char *path,
		t_scene_description *scene_description
		)
{
	static int	has_been_set_count = 0;

	if (has_been_set_count >= 4)
		return (print_error_return(INVALID_IDENTIFIER_ERROR, 1));
//	if (!is_readable_file(path))
//		return (EXIT_FAILURE);
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
	has_been_set_count += 1;
	return (EXIT_SUCCESS);
}

int	set_textures_and_colors_from_content(
		char ***scene_content,
		t_scene_description *scene_desc
		)
{
	char	**splitted;
	int		i;
	int		ret;

	i = 0;
	while (*scene_content && (*scene_content)[i] != NULL \
	&& !is_in_map((*scene_content)[i]))
	{
		ret = -1;
		splitted = ft_split_set((*scene_content)[i], " \t\n\v\f\r");
		if (splitted == NULL)
			return (print_error_return(ALLOC_ERROR, 1));
		if (is_texture(splitted[0]))
			ret = set_texture(splitted[0], splitted[1], scene_desc);
		else if (is_color(splitted[0]))
			ret = set_color(splitted[0], splitted[1], scene_desc);
		if (ret != 0 || ft_str_arr_len(splitted) != 2)
			print_error_exit(INVALID_IDENTIFIER_ERROR, 1, splitted);
		ft_free_split(splitted);
		i += 1;
	}
	*scene_content += i;
	return (EXIT_SUCCESS);
}
