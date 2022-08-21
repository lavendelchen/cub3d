/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_valid_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 22:26:07 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/08 22:39:59 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"
#include "../../../inc/parser.h"

static bool	is_open_wall(char *map[], int i)
{
	if (i == 0 || map[i + 1] == NULL)
	{
		if (ft_strchr(map[i], '0') != NULL \
		|| ft_strchr(map[i], 'N') != NULL || ft_strchr(map[i], 'S') != NULL \
		|| ft_strchr(map[i], 'E') != NULL || ft_strchr(map[i], 'W') != NULL)
			return (true);
	}
	if (map[i][0] != '1' || map[i][ft_strlen(map[i]) - 2] != '1')
		return (true);
	return (false);
}

bool	has_player(char *line)
{
	if (ft_strchr(line, 'N') == NULL && ft_strchr(line, 'S') == NULL \
	&& ft_strchr(line, 'E') == NULL && ft_strchr(line, 'W') == NULL)
		return (false);
	return (true);
}

static inline bool	line_has_valid_walls(
	char *line_to_check, int j, const char *original_line
	)
{
	if (line_to_check == NULL)
		return (true);
	if (j > ft_strlen(line_to_check) - 1)
		return (true);
	if (j != 0)
	{
		if (line_to_check[j - 1] != 'X' && line_to_check[j - 1] != '1')
			return (false);
	}
	if (j + 1 != ft_strlen(line_to_check) \
	&& j + 1 != ft_strlen(line_to_check) - 1)
	{
		if (line_to_check != original_line \
		&& (line_to_check[j] != 'X' && line_to_check[j] != '1'))
			return (false);
		if (line_to_check[j + 1] != 'X' && line_to_check[j + 1] != '1')
			return (false);
	}
	return (true);
}

bool	is_neighbor_to_wall_char(char *map[], int i, int j)
{
	if (i != 0 && !line_has_valid_walls(map[i - 1], j, map[i]))
		return (false);
	if (!line_has_valid_walls(map[i], j, map[i]))
		return (false);
	if (map[i + 1] != NULL && !line_has_valid_walls(map[i + 1], j, map[i]))
		return (false);
	return (true);
}

bool	has_valid_map(const char *scene_file_path)
{
	int		player_count;
	char	**map;
	int		i;

	player_count = 0;
	map = adjust_map(get_map((char *)scene_file_path));
	i = 0;
	while (map[i])
	{
		if ((map[i + 1] != NULL && map[i][ft_strlen(map[i]) - 1] != '\n') \
		|| is_open_wall(map, i) \
		|| (map[i + 1] != NULL && is_empty_line(map[i + 1])) \
		|| ft_strchr(map[i], ' ') != NULL)
			return (free_map_return(map, false));
		if (has_player(map[i]))
			player_count += 1;
		i += 1;
	}
	if (player_count != 1)
		return (free_map_return(map, false));
	return (free_map_return(map, true));
}
