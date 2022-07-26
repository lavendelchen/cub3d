/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_valid_walls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:50:56 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/24 00:14:26 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"
#include "../../../inc/parser.h"

bool	is_valid_map_char(char ch);
bool	is_wall(char ch);

static char	**getmap(const char *scene_file_path)
{
	char	**map;
	char	*line;
	int		fd;
	int		i;

	fd = open(scene_file_path, O_RDONLY);
	if (fd == -1)
		return (NULL);
	map = ft_calloc(ft_get_line_count((char *)scene_file_path) + 1, \
	sizeof(*map));
	if (map == NULL)
		return (NULL);
	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (is_in_map(line))
		{
			map[i] = ft_strdup(line);
			i += 1;
		}
		free(line);
		line = get_next_line(fd);
	}
	return (map);
}

static inline bool	is_valid_minus(char *map[], int i, int j)
{
	int	k;
	int	l;

	k = i;
	l = j;
	while (k >= 0 && l >= 0)
	{
		if (!is_valid_map_char(map[k][l]) || !is_valid_map_char(map[k][j]) \
		|| !is_valid_map_char(map[i][l]))
			return (false);
		if (is_wall(map[k][l]) || is_wall(map[k][j]) || is_wall(map[i][l]))
			break ;
		k -= 1;
		l -= 1;
	}
	return (true);
}

static inline bool	is_valid_plus(char *map[], int i, int j)
{
	int	k;
	int	l;

	k = i;
	l = j;
	while (map[k] && map[k][l])
	{
		if (!is_valid_map_char(map[k][l]) || !is_valid_map_char(map[k][j]) \
		|| !is_valid_map_char(map[i][l]))
			return (false);
		if (is_wall(map[k][l]) || is_wall(map[k][j]) || is_wall(map[i][l]))
			break ;
		k += 1;
		l += 1;
	}
	return (true);
}

static inline bool	is_valid(char *map[], int i, int j)
{
	if (map[i - 1])
	{
		if (ft_strlen(map[i - 1]) < j)
			return (false);
	}
	if (map[i + 1])
	{
		if (ft_strlen(map[i + 1]) < j)
			return (false);
	}
	return (is_valid_minus(map, i, j) && is_valid_plus(map, i, j));
}

bool	has_valid_walls(const char *scene_file_path)
{
	char	**map;
	int		i;
	int		j;

	map = getmap((char *)scene_file_path);
	if (map == NULL)
		return (false);
	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == '0' && !is_valid(map, i, j))
				return (free_map_return(map, false));
			j += 1;
		}
		i += 1;
	}
	ft_free_split(map);
	return (true);
}
