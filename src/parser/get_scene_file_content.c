/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_scene_file_content.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 16:29:01 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/08 22:25:17 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"
#include "../../inc/cub3d.h"
#include <fcntl.h>

bool	is_empty_line(const char *line)
{
	int	line_len;
	int	i;

	line_len = ft_strlen(line);
	i = 0;
	while (i < line_len)
	{
		if (ft_isspace(line[i]) == false)
			return (false);
		i += 1;
	}
	return (true);
}

static char	*iterate_file(int fd)
{
	char	*line;

	line = get_next_line(fd);
	if (line == NULL)
		return (NULL);
	if (ft_strlen(line) == 0 || is_empty_line(line))
	{
		free(line);
		return (iterate_file(fd));
	}
	return (line);
}

static bool	identifier_is_cut_by_newline(char *identifier)
{
	int	identifier_len;

	if (ft_strchr(identifier, '\n') == NULL)
		return (false);
	identifier_len = ft_strlen(identifier);
	if (identifier_len > 3)
		return (false);
	if (identifier_len == 3)
	{
		if (ft_strcmp(identifier, "NO\n") != 0 \
		&& ft_strcmp(identifier, "SO\n") != 0 \
		&& ft_strcmp(identifier, "WE\n") != 0 \
		&& ft_strcmp(identifier, "EA\n") != 0)
			return (false);
	}
	if (identifier_len == 2)
	{
		if (ft_strcmp(identifier, "C\n") != 0 \
		&& ft_strcmp(identifier, "F\n") != 0)
			return (false);
	}
	return (true);
}

static void	adjust_identifier_cut_by_newline(
		char **scene_file_content_ref,
		const int fd
		)
{
	char	*next_line;

	next_line = iterate_file(fd);
	*scene_file_content_ref = ft_append(scene_file_content_ref, next_line);
	if (*scene_file_content_ref == NULL)
		exit(-1);
	free(next_line);
}

char	**get_scene_file_content(
		const char *scene_file_path
)
{
	char	**scene_file_content;
	int		fd;
	int		i;

	scene_file_content = ft_calloc(
			ft_get_line_count((char *)scene_file_path) + 1, sizeof (char *));
	if (scene_file_content == NULL)
		return (NULL);
	fd = open(scene_file_path, O_RDONLY);
	if (fd == -1)
		return (NULL);
	i = 0;
	scene_file_content[i] = iterate_file(fd);
	while (scene_file_content[i] != NULL)
	{
		if (identifier_is_cut_by_newline(scene_file_content[i]))
			adjust_identifier_cut_by_newline(&scene_file_content[i], fd);
		i += 1;
		scene_file_content[i] = iterate_file(fd);
	}
	close(fd);
	return (scene_file_content);
}
