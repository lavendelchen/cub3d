/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 13:17:46 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/02 16:31:09 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include "../../inc/parser.h"

static inline bool	is_readable_file(const char *path_to_file)
{
	int	fd;

	fd = open(path_to_file, O_RDONLY);
	if (fd == -1)
		return (false);
	close(fd);
	return (true);
}

int print_error_return(const char *error_message, int return_code)
{
	ft_putstr_fd((char *)error_message, STDERR_FILENO);
	return (return_code);
}

int	parser(
		const char *scene_file_path,
		t_scene_description *scene_description
		)
{
	char	**scene_file_content;

	if (scene_file_path == NULL || scene_description == NULL)
		return (print_error_return(ARG_NULL_ERROR, 1));
	if (!is_readable_file(scene_file_path))
		return (print_error_return(NON_READABLE_FILE_ERROR, 1));
	if (!has_valid_map(scene_file_path))
		return (print_error_return(INVALID_MAP_ERROR, 1));
	scene_file_content = get_scene_file_content(scene_file_path);
	if (scene_file_content == NULL)
		return (print_error_return(ALLOC_ERROR, 1));
	if (get_scene_description_from_content(scene_file_content, scene_description) != 0)
		return (EXIT_FAILURE);
	ft_free_split(scene_file_content);
	return (EXIT_SUCCESS);
}
