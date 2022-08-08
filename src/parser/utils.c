/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 22:29:03 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/08 22:30:12 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include "../../inc/parser.h"

bool	free_map_return(char *map[], bool return_value)
{
	ft_free_split(map);
	return (return_value);
}

void	print_error_exit(char *error_message, int exit_code, void *to_free)
{
	free(to_free);
	ft_putstr_fd(error_message, STDERR_FILENO);
	exit(exit_code);
}

bool	is_in_map(const char *line)
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
