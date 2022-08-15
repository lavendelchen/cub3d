/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map_from_content.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 22:42:22 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/08 22:42:54 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"
#include "../../../inc/parser.h"

static char	*get_valid_map_line(char *map_line)
{
	char	*valid_line;
	int		i;

	if (map_line == NULL)
		return (NULL);
	valid_line = ft_calloc(ft_strlen(map_line) + 1, sizeof (*map_line));
	if (valid_line == NULL)
		return (NULL);
	i = 0;
	while (map_line[i] != '\0')
	{
		if (map_line[i] == ' ' || map_line[i] == '\n')
			valid_line[i] = '1';
		else
			valid_line[i] = map_line[i];
		i += 1;
	}
	return (valid_line);
}

static inline enum e_direction	get_direction(char player_pos)
{
	if (player_pos == 'N')
		return (NO);
	if (player_pos == 'S')
		return (SO);
	if (player_pos == 'E')
		return (EA);
	if (player_pos == 'W')
		return (WE);
	return (-1);
}

/**
 * Sets the Position of the Player in the Level
 * @details X refers to the horizontal Position, east west
 * @details Y refers to the vertical Position, up down
 * @details Z refers to the depth Position, north east
 */
static void	get_player(
		t_scene_description *scene_desc,
		char *line,
		int z
		)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == 'N' || line[i] == 'S' || line[i] == 'E' || line[i] == 'W')
		{
			scene_desc->player.x = i;
			scene_desc->player.y = 0;
			scene_desc->player.z = z;
			scene_desc->player.direction = get_direction(line[i]);
			break ;
		}
		i += 1;
	}
}

int	set_map_from_content(
		char **scene_content,
		t_scene_description *scene_desc
		)
{
	int	i;

	scene_desc->map_content = ft_calloc(ft_str_arr_len(scene_content) + 1, \
	sizeof(*scene_desc->map_content));
	if (scene_desc->map_content == NULL)
		return (print_error_return(ALLOC_ERROR, 1));
	i = 0;
	while (scene_content[i] != NULL)
	{
		if (has_player(scene_content[i]))
			get_player(scene_desc, scene_content[i], i);
		scene_desc->map_content[i] = get_valid_map_line(scene_content[i]);
		if (scene_desc->map_content[i] == NULL)
			return (print_error_return(ALLOC_ERROR, 1));
		i += 1;
	}
	return (EXIT_SUCCESS);
}
