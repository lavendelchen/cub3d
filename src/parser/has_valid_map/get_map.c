/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 22:26:53 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/08 22:39:51 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"
#include "../../../inc/parser.h"

bool	is_neighbor_to_wall_char(char *map[], int i, int j);

static char	*get_valid_map_line(char **line)
{
	char	*line_buf;
	char	*valid_line;
	int		i;

	line_buf = *line;
	if (ft_strchr(line_buf, ' ') == NULL)
		return (ft_strdup(line_buf));
	valid_line = ft_calloc(ft_strlen(line_buf) + 1, sizeof (*valid_line));
	if (valid_line == NULL)
		print_error_exit(ALLOC_ERROR, -1, line_buf);
	i = 0;
	while (line_buf[i] != '\0')
	{
		if (ft_isspace(line_buf[i]) && line_buf[i] != '\n')
			valid_line[i] = 'X';
		else
			valid_line[i] = line_buf[i];
		i += 1;
	}
	free(*line);
	*line = NULL;
	return (valid_line);
}

static char	*replace_by(char *line, char to_replace, char replace_with)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == to_replace)
			line[i] = replace_with;
		else
			line[i] = line[i];
		i += 1;
	}
	return (line);
}

static inline bool	can_be_replaced(char *map[], int i)
{
	int	j;

	j = 0;
	while (map[i][j] != '\0')
	{
		if (map[i][j] == 'X' && !is_neighbor_to_wall_char(map, i, j))
			return (false);
		j += 1;
	}
	return (true);
}

char	**adjust_map(char *map[])
{
	int	i;

	i = 0;
	while (map[i])
	{
		if (ft_strchr(map[i], 'X'))
		{
			if (can_be_replaced(map, i))
				map[i] = replace_by(map[i], 'X', '1');
			else
				map[i] = replace_by(map[i], 'X', ' ');
		}
		i += 1;
	}
	return (map);
}

char	**get_map(char *scene_file_path)
{
	int		fd;
	int		i;
	char	**map;
	char	*line;

	fd = open(scene_file_path, O_RDONLY);
	if (fd == -1)
		return (NULL);
	map = ft_calloc(ft_get_line_count(scene_file_path) + 1, sizeof (*map));
	if (map == NULL)
		print_error_exit(ALLOC_ERROR, -1, NULL);
	i = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (is_in_map(line))
		{
			map[i] = get_valid_map_line(&line);
			i += 1;
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (map);
}
