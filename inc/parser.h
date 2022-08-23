/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 16:26:18 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/23 19:25:32 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# define ALLOC_ERROR "Error\nAllocation Error\n"
# define ARG_NULL_ERROR "Error\nArgument is NULL\n"
# define NON_READABLE_FILE_ERROR "Error\nFile is not existent / invalid\n"
# define INVALID_MAP_ERROR "Error\nInvalid Map in .cub file\n"
# define INVALID_IDENTIFIER_ERROR "Error\nInvalid Identifier in .cub file\n"
# define INVALID_COLOR_ERROR "Error\nInvalid Color in .cub file\n"
# define INVALID_MAP_ERROR "Error\nInvalid Map in .cub file\n"
# define INVALID_TEXTURE_PATH "Error\nInvalid Texture Path in .cub file\n"

# include <fcntl.h>
# include <stdbool.h>

typedef struct s_scene_description	t_scene_description;

/* has_valid_map.c */
bool	has_valid_map(const char *scene_file_path);

/* get_map.c */
char	**adjust_map(char *map[]);
char	**get_map(char *scene_file_path);

/* get_scene_description */
int		set_map_from_content(char **content, t_scene_description *scene_desc);
int		set_textures_and_colors_from_content(
		char ***scene_content, t_scene_description *scene_desc
);
bool	is_valid_color_description(char *desc);
int		set_floor_color(
		t_scene_description *scene_description,
		int r, int g, int b
);
int		set_ceiling_color(
		t_scene_description *scene_description,
		int r, int g, int b
);

/* utils.c */
bool	free_map_return(char *map[], bool return_value);
void	print_error_exit(char *error_message, int exit_code, void *to_free);
bool	is_in_map(const char *line);
bool	has_player(char *line);


char	**get_scene_file_content(const char *scene_file_path);
bool	is_readable_file(const char *path_to_file);
int		get_scene_description_from_content(
			char **scene_file_content, t_scene_description *scene_description);
bool	is_empty_line(const char *line);
int		print_error_return(const char *error_message, int return_code);
bool	has_valid_map(const char *scene_file_path);
bool	has_valid_identifiers(const char *scene_file_path);

#endif
