/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_non_empty_lines_after_map.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 23:28:20 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/24 00:25:59 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"
#include "../../../inc/parser.h"

static char	**get_scene_content(const char *scene_file_path)
{
	char	**map;
	int		fd;
	int		i;
	char	*line;

	fd = open(scene_file_path, O_RDONLY);
	if (fd == -1)
		return (NULL);
	map = ft_calloc(ft_get_line_count((char *)scene_file_path) + 1, \
	sizeof(*map));
	if (map == NULL)
		return (NULL);
	line = get_next_line(fd);
	i = 0;
	while (line)
	{
		map[i] = ft_strdup(line);
		free(line);
		line = get_next_line(fd);
		i += 1;
	}
	close(fd);
	return (map);
}

static inline void	iterate_content(
	char **scene_content, int *i, bool *was_in_map
	)
{
	while (scene_content[*i])
	{
		while (scene_content[*i] && is_in_map(scene_content[*i]))
		{
			*was_in_map = true;
			*i += 1;
		}
		if (*was_in_map)
			break ;
		*i += 1;
	}
}

bool	has_non_empty_lines_after_map(const char *scene_file_path)
{
	char	**scene_content;
	int		i;
	int		j;
	bool	was_in_map;

	scene_content = get_scene_content(scene_file_path);
	if (scene_content == NULL)
		print_error_exit(ALLOC_ERROR, -1, NULL);
	i = 0;
	was_in_map = false;
	iterate_content(scene_content, &i, &was_in_map);
	while (was_in_map && scene_content[i])
	{
		j = 0;
		while (scene_content[i][j])
		{
			if (ft_isalnum(scene_content[i][j]))
				return (free_map_return(scene_content, true));
			j += 1;
		}
		i += 1;
	}
	return (free_map_return(scene_content, false));
}
