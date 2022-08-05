/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_valid_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 15:41:45 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/03 15:41:46 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include "../../inc/parser.h"

static bool	next_line_empty(int fd)
{
	char	*line;

	line = get_next_line(fd);
	if (line == NULL)
		return (false);
	if (is_empty_line(line))
	{
		free(line);
		return (true);
	}
	free(line);
	return (false);
}

static bool	is_in_map(const char *line)
{
	if (is_empty_line(line))
		return (false);
	if (ft_strncmp(line, "NO", 2) == 0 || ft_strncmp(line, "SO", 2) == 0
		|| ft_strncmp(line, "EA", 2) == 0 || ft_strncmp(line, "WE", 2) == 0)
		return (false);
	if (ft_strncmp(line, "C", 1) == 0 || ft_strncmp(line, "F", 1) == 0)
		return (false);
	if (ft_strchr(line, '1') == NULL && ft_strchr(line, '0') == NULL
		&& ft_strchr(line, 'N') == NULL && ft_strchr(line, 'S') == NULL
		&& ft_strchr(line, 'E') == NULL && ft_strchr(line, 'W') == NULL)
		return (false);
	return (true);
}


bool	has_valid_map(const char *scene_file_path)
{
	int 	fd;
	char	*line;

	fd = open(scene_file_path, O_RDONLY);
	if (fd == -1)
		return (false);
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (is_in_map(line) && next_line_empty(fd))
				return (false);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (true);
}