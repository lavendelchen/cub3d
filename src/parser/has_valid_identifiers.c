/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_valid_identifiers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 22:10:25 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/08 22:34:40 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"
#include "../../inc/cub3d.h"

static char	**get_identifiers(char *scene_file_path)
{
	char	**identifiers;
	char	*line;
	int		i;
	int		fd;

	fd = open(scene_file_path, O_RDONLY);
	if (fd == -1)
		return (NULL);
	identifiers = ft_calloc(ft_get_line_count(scene_file_path) + 1, \
	sizeof(*identifiers));
	if (identifiers == NULL)
		return (NULL);
	i = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (!is_empty_line(line) && is_in_map(line))
			break ;
		identifiers[i++] = ft_strdup(line);
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
	return (identifiers);
}

#define FLOOR 0
#define CEILING 1

static bool	is_correct_identifier_count(char *identifier)
{
	static int	texture_count[4];
	static int	color_count[2];

	if (identifier != NULL && ft_strstr(identifier, "NO"))
		texture_count[NO] += 1;
	if (identifier != NULL && ft_strstr(identifier, "SO"))
		texture_count[SO] += 1;
	if (identifier != NULL && ft_strstr(identifier, "EA"))
		texture_count[EA] += 1;
	if (identifier != NULL && ft_strstr(identifier, "WE"))
		texture_count[WE] += 1;
	if (identifier != NULL && ft_strstr(identifier, "F"))
		color_count[FLOOR] += 1;
	if (identifier != NULL && ft_strstr(identifier, "C"))
		color_count[CEILING] += 1;
	if ((texture_count[NO] > 1 || texture_count[SO] > 1 \
	|| texture_count[EA] > 1 || texture_count[WE] > 1) \
	|| (color_count[FLOOR] > 1 || color_count[CEILING] > 1))
		return (false);
	if ((identifier == NULL) \
	&& ((texture_count[NO] != 1 || texture_count[SO] != 1 \
	|| texture_count[EA] != 1 || texture_count[WE] != 1) \
	|| (color_count[FLOOR] != 1 || color_count[CEILING] != 1)))
		return (false);
	return (true);
}

bool	has_valid_identifiers(const char *scene_file_path)
{
	char	**identifiers;
	int		i;

	identifiers = get_identifiers((char *)scene_file_path);
	if (identifiers == NULL)
		return (false);
	i = 0;
	while (identifiers[i])
	{
		if (!is_correct_identifier_count(identifiers[i]))
		{
			ft_free_split(identifiers);
			return (false);
		}
		i += 1;
	}
	if (!is_correct_identifier_count(NULL))
	{
		ft_free_split(identifiers);
		return (false);
	}
	ft_free_split(identifiers);
	return (true);
}
