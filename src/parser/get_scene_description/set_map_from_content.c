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
		scene_desc->map_content[i] = ft_strdup(scene_content[i]);
		if (scene_desc->map_content[i] == NULL)
			return (print_error_return(ALLOC_ERROR, 1));
		i += 1;
	}
	return (EXIT_SUCCESS);
}
