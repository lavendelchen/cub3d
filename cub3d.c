/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 20:09:31 by shaas             #+#    #+#             */
/*   Updated: 2022/08/24 01:15:44 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./inc/cub3d.h"
#include "./inc/parser.h"

static inline bool	init_directions_no_so(
	int starting_direction, t_vectors *vectors
	)
{
	if (starting_direction == NO)
	{
		vectors->player_direction[X] = 0;
		vectors->player_direction[Y] = -1;
		vectors->camera_plane[X] = FOV;
		vectors->camera_plane[Y] = 0;
		return (true);
	}
	else if (starting_direction == SO)
	{
		vectors->player_direction[X] = 0;
		vectors->player_direction[Y] = 1;
		vectors->camera_plane[X] = FOV * -1;
		vectors->camera_plane[Y] = 0;
		return (true);
	}
	return (false);
}

static inline void	init_directions_we_ea(
	int starting_direction, t_vectors *vectors
	)
{
	if (starting_direction == WE)
	{
		vectors->player_direction[X] = -1;
		vectors->player_direction[Y] = 0;
		vectors->camera_plane[X] = 0;
		vectors->camera_plane[Y] = FOV * -1;
	}
	else if (starting_direction == EA)
	{
		vectors->player_direction[X] = 1;
		vectors->player_direction[Y] = 0;
		vectors->camera_plane[X] = 0;
		vectors->camera_plane[Y] = FOV;
	}
}

/*
player_position gets added 0.5 so we don't start at the corner
of the starting tile, but the middle of it
*/
int	init_game(t_game *game, t_scene_description *scene_desc)
{
	game->vectors.player_position[X]
		= scene_desc->player.position[X] + 0.5;
	game->vectors.player_position[Y]
		= scene_desc->player.position[Y] + 0.5;
	if (!init_directions_no_so(
			scene_desc->player.direction, &(game->vectors)))
		init_directions_we_ea(
			scene_desc->player.direction, &(game->vectors));
	game->wall[NO] = mlx_load_png(scene_desc->textures[NO]);
	game->wall[SO] = mlx_load_png(scene_desc->textures[SO]);
	game->wall[WE] = mlx_load_png(scene_desc->textures[WE]);
	game->wall[EA] = mlx_load_png(scene_desc->textures[EA]);
	if (!game->wall[NO] || !game->wall[SO]
		|| !game->wall[WE] || !game->wall[EA])
		return (put_error_msg("Error\nPNG could not be loaded\n"));
	game->mlx_ptr = mlx_init(SCREENWIDTH, SCREENHEIGHT, "🌈RainbowCube🌈", false);
	if (!game->mlx_ptr)
		return (put_error_msg("Error\nMlx couldn't be initiated\n"));
	game->mlx_img = mlx_new_image(game->mlx_ptr, SCREENWIDTH, SCREENHEIGHT);
	if (!game->mlx_img)
		return (put_error_msg("Error\nMlx image couldn't be allocated\n"));
	if (mlx_image_to_window(game->mlx_ptr, game->mlx_img, 0, 0) == -1)
		return (put_error_msg("Error\nImage couldn't be put to window\n"));
	init_music(scene_desc->textures[scene_desc->player.direction]);
	return (EXIT_SUCCESS);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/24 01:25:46 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


int	main(int argc, const char *argv[])
{
	t_scene_description	scene_desc;
	t_game				game;
	t_bundle			bundle;

	if (argc != 2)
	{
		ft_putstr_fd("Error\nOne Argument Needed\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (parser(argv[1], &scene_desc) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	bundle.game = &game;
	bundle.scene_desc = &scene_desc;
	if (init_game(&game, &scene_desc) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	mlx_key_hook(game.mlx_ptr, close_at_esc, &bundle);
	mlx_close_hook(game.mlx_ptr, free_at_window_close, &bundle);
	if (!mlx_loop_hook(game.mlx_ptr, raycasting_loop, &bundle))
		return (put_error_msg("Error\nMlx hook couldn't be added\n"));
	mlx_loop(game.mlx_ptr);
	mlx_terminate(game.mlx_ptr);
	if (MUSIC)
		kill_music();
	return (EXIT_SUCCESS);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 16:09:23 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/23 21:03:47 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


static inline void	move(
	double new_player_y, double new_player_x, char **map, t_vectors *vectors
	)
{
	if (map[(int)new_player_y][(int)new_player_x] != '1')
	{
		vectors->player_position[Y] = new_player_y;
		vectors->player_position[X] = new_player_x;
	}
}

void	move_left_right(
	t_vectors *vectors, char **map, mlx_t *mlx_ptr)
{
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_D))
	{
		move(vectors->player_position[Y],
			vectors->player_position[X]
			- (vectors->player_direction[Y] * MOVESPEED), map, vectors);
		move(vectors->player_position[Y]
			+ (vectors->player_direction[X] * MOVESPEED),
			vectors->player_position[X], map, vectors);
	}
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_A))
	{
		move(vectors->player_position[Y],
			vectors->player_position[X]
			+ (vectors->player_direction[Y] * MOVESPEED), map, vectors);
		move(vectors->player_position[Y]
			- (vectors->player_direction[X] * MOVESPEED),
			vectors->player_position[X], map, vectors);
	}
}

void	move_forward_back(
	struct s_vectors *vectors, char **map, mlx_t *mlx_ptr)
{
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_W))
	{
		move(vectors->player_position[Y],
			vectors->player_position[X]
			+ (vectors->player_direction[X] * MOVESPEED), map, vectors);
		move(
			vectors->player_position[Y]
			+ (vectors->player_direction[Y] * MOVESPEED),
			vectors->player_position[X], map, vectors);
	}
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_S))
	{
		move(vectors->player_position[Y],
			vectors->player_position[X]
			- (vectors->player_direction[X] * MOVESPEED), map, vectors);
		move(vectors->player_position[Y]
			- (vectors->player_direction[Y] * MOVESPEED),
			vectors->player_position[X], map, vectors);
	}
}

void	check_movement(struct s_vectors *vectors, char **map, mlx_t *mlx_ptr)
{
	move_forward_back(vectors, map, mlx_ptr);
	move_left_right(vectors, map, mlx_ptr);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 16:10:46 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/23 21:04:03 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


static inline void	rotate_left(
	t_vectors *vectors,
	const double *old_player_dir, const double *old_camera_plane
	)
{
	vectors->player_direction[X] = vectors->player_direction[X] \
	* cos(-ROTSPEED) - vectors->player_direction[Y] * sin(-ROTSPEED);
	vectors->player_direction[Y] = old_player_dir[X] * sin(-ROTSPEED) \
	+ vectors->player_direction[Y] * cos(-ROTSPEED);
	vectors->camera_plane[X] = vectors->camera_plane[X] \
	* cos(-ROTSPEED) - vectors->camera_plane[Y] * sin(-ROTSPEED);
	vectors->camera_plane[Y] = old_camera_plane[X] * sin(-ROTSPEED) \
	+ vectors->camera_plane[Y] * cos(-ROTSPEED);
}

static inline void	rotate_right(
	t_vectors *vectors,
	const double *old_player_dir, const double *old_camera_plane
	)
{
	vectors->player_direction[X] = vectors->player_direction[X] * cos(ROTSPEED) \
	- vectors->player_direction[Y] * sin(ROTSPEED);
	vectors->player_direction[Y] = old_player_dir[X] * sin(ROTSPEED) \
	+ vectors->player_direction[Y] * cos(ROTSPEED);
	vectors->camera_plane[X] = vectors->camera_plane[X] \
	* cos(ROTSPEED) - vectors->camera_plane[Y] * sin(ROTSPEED);
	vectors->camera_plane[Y] = old_camera_plane[X] * sin(ROTSPEED) \
	+ vectors->camera_plane[Y] * cos(ROTSPEED);
}

void	check_rotation(struct s_vectors *vectors, mlx_t *mlx_ptr)
{
	double	old_player_dir[1];
	double	old_camera_plane[1];

	old_player_dir[X] = vectors->player_direction[X];
	old_camera_plane[X] = vectors->camera_plane[X];
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_LEFT))
		rotate_left(vectors, old_player_dir, old_camera_plane);
	if (mlx_is_key_down(mlx_ptr, MLX_KEY_RIGHT))
		rotate_right(vectors, old_player_dir, old_camera_plane);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   music.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 01:15:19 by shaas             #+#    #+#             */
/*   Updated: 2022/08/24 01:46:14 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#define START_LOOP "zsh -c \"while true; do; afplay "
#define NYANCAT "./music/nyan-cat.mp3; "
#define FUNDISCO "./music/fun-disco.mp3; "
#define GARDEN "./music/peaceful-garden.mp3; "
#define CAVE "./music/droplets-in-a-cave.mp3; "
#define END_LOOP "done &\""

void	kill_music(void)
{
	system("kill 0");
}

void	init_music(char *start_texture)
{
	if (!MUSIC)
		return ;
	if (ft_strstr(start_texture, "flag"))
		system(START_LOOP NYANCAT END_LOOP);
	else if (ft_strstr(start_texture, "arcade"))
		system(START_LOOP FUNDISCO END_LOOP);
	else if (ft_strstr(start_texture, "cloud"))
		system(START_LOOP GARDEN END_LOOP);
	else if (ft_strstr(start_texture, "ground"))
		system(START_LOOP CAVE END_LOOP);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_scene_description_from_content.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 17:49:01 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/08 22:46:44 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


int	set_ceiling_color(
		t_scene_description *scene_description,
		int r, int g, int b
)
{
	static bool	has_been_set = false;

	if (has_been_set)
		return (print_error_return(INVALID_IDENTIFIER_ERROR, 1));
	if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255))
		return (print_error_return(INVALID_COLOR_ERROR, 1));
	scene_description->ceiling_color = rgba(r, g, b, 0xFF);
	has_been_set = true;
	return (EXIT_SUCCESS);
}

int	set_floor_color(
		t_scene_description *scene_description,
		int r, int g, int b
		)
{
	static bool	has_been_set = false;

	if (has_been_set)
		return (print_error_return(INVALID_IDENTIFIER_ERROR, 1));
	if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255))
		return (print_error_return(INVALID_COLOR_ERROR, 1));
	scene_description->floor_color = rgba(r, g, b, 0xFF);
	has_been_set = true;
	return (EXIT_SUCCESS);
}

bool	is_valid_color_description(char *desc)
{
	int	i;

	if (ft_chrcount(desc, ',') != 2)
		return (false);
	i = 0;
	while (desc[i] != '\0')
	{
		if (desc[i] == ',')
		{
			if (i == 0 || desc[i + 1] == '\0')
				return (false);
			if (!ft_isdigit(desc[i - 1]) && !ft_isalnum(desc[i + 1]))
				return (false);
		}
		else if (!ft_isdigit(desc[i]))
			return (false);
		i += 1;
	}
	return (true);
}

int	get_scene_description_from_content(
		char **scene_file_content,
		t_scene_description *scene_description
)
{
	if (set_textures_and_colors_from_content(
			&scene_file_content, scene_description
		) != 0)
		return (print_error_return("Error\nInvalid Texture / Color\n", 1));
	if (set_map_from_content(
			scene_file_content, scene_description
		) != 0)
		return (print_error_return(INVALID_MAP_ERROR, 1));
	return (EXIT_SUCCESS);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map_from_content.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 22:42:22 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/21 16:57:00 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


static char	*get_valid_map_line_single(char *map_line)
{
	char	*valid_line;
	int		i;

	if (map_line == NULL)
		return (NULL);
	valid_line = ft_calloc(ft_strlen(map_line) + 1, sizeof (*map_line));
	if (valid_line == NULL)
		return (NULL);
	i = 0;
	while (map_line[i] != '\0')
	{
		if (map_line[i] == ' ' || map_line[i] == '\n')
			valid_line[i] = '1';
		else
			valid_line[i] = map_line[i];
		i += 1;
	}
	return (valid_line);
}

static inline enum e_direction	get_direction(char player_pos)
{
	if (player_pos == 'N')
		return (NO);
	if (player_pos == 'S')
		return (SO);
	if (player_pos == 'E')
		return (EA);
	if (player_pos == 'W')
		return (WE);
	return (-1);
}

/**
 * Sets the Position of the Player
 * @param line The Line where the Player was found
 * @param z North South Position of Player
 * @arg line_iterator Iterates the line
 *		Corresponding to the West East Position of the Player,
 */
static void	get_player(
		t_scene_description *scene_desc,
		char *line,
		int y
		)
{
	int	line_iterator;

	line_iterator = 0;
	while (line[line_iterator] != '\0')
	{
		if (line[line_iterator] == 'N' || line[line_iterator] == 'S' \
		|| line[line_iterator] == 'E' || line[line_iterator] == 'W')
		{
			scene_desc->player.position[X] = line_iterator;
			scene_desc->player.position[Y] = y;
			scene_desc->player.direction = get_direction(line[line_iterator]);
			break ;
		}
		line_iterator += 1;
	}
}

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
		if (has_player(scene_content[i]))
			get_player(scene_desc, scene_content[i], i);
		scene_desc->map_content[i] = get_valid_map_line_single(scene_content[i]);
		if (scene_desc->map_content[i] == NULL)
			return (print_error_return(ALLOC_ERROR, 1));
		i += 1;
	}
	return (EXIT_SUCCESS);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_textures_and_color.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 22:44:09 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/22 19:38:17 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


static bool	is_color(char *identifier)
{
	if (ft_strlen(identifier) != 1)
		return (false);
	if (identifier[0] != 'C' && identifier[0] != 'F')
		return (false);
	return (true);
}

static bool	is_texture(char *identifier)
{
	if (ft_strlen(identifier) != 2)
		return (false);
	if (ft_strcmp(identifier, "NO") != 0 && ft_strcmp(identifier, "SO") != 0
		&& ft_strcmp(identifier, "WE") != 0 && ft_strcmp(identifier, "EA") != 0)
		return (false);
	return (true);
}

static int	set_color(
		char *identifier,
		char *rbg,
		t_scene_description *scene_description
		)
{
	char	**rbg_splitted;
	int		ret;

	ret = -1;
	if (!is_valid_color_description(rbg))
		return (EXIT_FAILURE);
	rbg_splitted = ft_split_set(rbg, " \t\n\v\f\r,");
	if (rbg_splitted == NULL)
		return (print_error_return(ALLOC_ERROR, 1));
	if (ft_strcmp(identifier, "F") == 0)
		ret = set_floor_color(
				scene_description,
				ft_atoi(rbg_splitted[0]),
				ft_atoi(rbg_splitted[1]),
				ft_atoi(rbg_splitted[2])
				);
	if (ft_strcmp(identifier, "C") == 0)
		ret = set_ceiling_color(
				scene_description,
				ft_atoi(rbg_splitted[0]),
				ft_atoi(rbg_splitted[1]),
				ft_atoi(rbg_splitted[2])
				);
	ft_free_split(rbg_splitted);
	return (ret);
}

static int	set_texture(
		char *identifier,
		char *path,
		t_scene_description *scene_description
		)
{
	static int	has_been_set_count = 0;

	if (has_been_set_count >= 4)
		return (print_error_return(INVALID_IDENTIFIER_ERROR, 1));
	if (!is_readable_file(path))
		return (print_error_return(INVALID_TEXTURE_PATH, 1));
	if (ft_strcmp(identifier, "NO") == 0)
		scene_description->textures[NO] = ft_strdup(path);
	else if (ft_strcmp(identifier, "SO") == 0)
		scene_description->textures[SO] = ft_strdup(path);
	else if (ft_strcmp(identifier, "WE") == 0)
		scene_description->textures[WE] = ft_strdup(path);
	else if (ft_strcmp(identifier, "EA") == 0)
		scene_description->textures[EA] = ft_strdup(path);
	else
		return (print_error_return(INVALID_IDENTIFIER_ERROR, 1));
	has_been_set_count += 1;
	return (EXIT_SUCCESS);
}

int	set_textures_and_colors_from_content(
		char ***scene_content,
		t_scene_description *scene_desc
		)
{
	char	**splitted;
	int		i;
	int		ret;

	i = 0;
	while (*scene_content && (*scene_content)[i] != NULL \
	&& !is_in_map((*scene_content)[i]))
	{
		ret = -1;
		splitted = ft_split_set((*scene_content)[i], " \t\n\v\f\r");
		if (splitted == NULL)
			return (print_error_return(ALLOC_ERROR, 1));
		if (is_texture(splitted[0]))
			ret = set_texture(splitted[0], splitted[1], scene_desc);
		else if (is_color(splitted[0]))
			ret = set_color(splitted[0], splitted[1], scene_desc);
		if (ret != 0 || ft_str_arr_len(splitted) != 2)
			print_error_exit(INVALID_IDENTIFIER_ERROR, 1, splitted);
		ft_free_split(splitted);
		i += 1;
	}
	*scene_content += i;
	return (EXIT_SUCCESS);
}
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_valid_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 22:26:07 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/24 00:26:26 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


static bool	is_open_wall(char *map[], int i)
{
	if (i == 0 || map[i + 1] == NULL)
	{
		if (ft_strchr(map[i], '0') != NULL \
		|| ft_strchr(map[i], 'N') != NULL || ft_strchr(map[i], 'S') != NULL \
		|| ft_strchr(map[i], 'E') != NULL || ft_strchr(map[i], 'W') != NULL)
			return (true);
	}
	if (map[i][0] != '1' || map[i][ft_strlen(map[i]) - 2] != '1')
		return (true);
	return (false);
}

bool	has_player(char *line)
{
	if (ft_strchr(line, 'N') == NULL && ft_strchr(line, 'S') == NULL \
	&& ft_strchr(line, 'E') == NULL && ft_strchr(line, 'W') == NULL)
		return (false);
	return (true);
}

bool	line_has_valid_walls(
	char *line_to_check, int j, const char *original_line
	)
{
	if (line_to_check == NULL)
		return (true);
	if (j > ft_strlen(line_to_check) - 1)
		return (true);
	if (j != 0)
	{
		if (line_to_check[j - 1] != 'X' && line_to_check[j - 1] != '1')
			return (false);
	}
	if (j + 1 != ft_strlen(line_to_check) \
	&& j + 1 != ft_strlen(line_to_check) - 1)
	{
		if (line_to_check != original_line \
		&& (line_to_check[j] != 'X' && line_to_check[j] != '1'))
			return (false);
		if (line_to_check[j + 1] != 'X' && line_to_check[j + 1] != '1')
			return (false);
	}
	return (true);
}

static inline bool	has_valid_char(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '0' || line[i] == '1' || line[i] == 'N' \
		|| line[i] == 'S' || line[i] == 'E' || line[i] == 'W' \
		|| line[i] == '\n')
		{
			i += 1;
			continue ;
		}
		return (false);
	}
	return (true);
}

bool	has_valid_map(const char *scene_file_path)
{
	int		player_count;
	char	**map;
	int		i;

	player_count = 0;
	map = adjust_map(get_map((char *)scene_file_path));
	i = 0;
	while (map[i])
	{
		if (!has_valid_char(map[i]))
			return (free_map_return(map, false));
		if ((map[i + 1] != NULL && map[i][ft_strlen(map[i]) - 1] != '\n') \
		|| is_open_wall(map, i) \
		|| (map[i + 1] != NULL && is_empty_line(map[i + 1])) \
		|| ft_strchr(map[i], ' ') != NULL)
			return (free_map_return(map, false));
		if (has_player(map[i]))
			player_count += 1;
		i += 1;
	}
	if (!has_valid_walls(scene_file_path) \
	|| has_non_empty_lines_after_map(scene_file_path) || player_count != 1)
		return (free_map_return(map, false));
	return (free_map_return(map, true));
}
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 21:40:50 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/23 21:43:34 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


bool	is_valid_map_char(char ch)
{
	if (ch == '0')
		return (true);
	if (ch == '1')
		return (true);
	if (ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W')
		return (true);
	if (ch == '\n')
		return (true);
	if (ft_isspace(ch))
		return (false);
	return (false);
}

bool	is_wall(char ch)
{
	return (ch == '1');
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 13:17:46 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/23 19:37:50 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


inline bool	is_readable_file(const char *path_to_file)
{
	int	fd;

	fd = open(path_to_file, O_RDONLY);
	if (fd == -1)
		return (false);
	close(fd);
	return (true);
}

static inline bool	has_valid_ext(const char *path_to_file)
{
	static char	*extension = ".cub";
	char		*ptr_to_extension;

	ptr_to_extension = ft_strstr(path_to_file, extension);
	if (ptr_to_extension == NULL)
		return (false);
	if (ft_strncmp(ptr_to_extension, extension, ft_strlen(extension) + 1) != 0)
		return (false);
	return (true);
}

int	print_error_return(const char *error_message, int return_code)
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
	if (!is_readable_file(scene_file_path) || !has_valid_ext(scene_file_path))
		return (print_error_return(NON_READABLE_FILE_ERROR, 1));
	if (!has_valid_identifiers(scene_file_path))
		return (print_error_return(INVALID_IDENTIFIER_ERROR, 1));
	if (!has_valid_map(scene_file_path))
		return (print_error_return(INVALID_MAP_ERROR, 1));
	scene_file_content = get_scene_file_content(scene_file_path);
	if (scene_file_content == NULL)
		return (print_error_return(ALLOC_ERROR, 1));
	if (get_scene_description_from_content(scene_file_content,
			scene_description) != 0)
		return (EXIT_FAILURE);
	ft_free_split(scene_file_content);
	return (EXIT_SUCCESS);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 22:29:03 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/23 21:36:39 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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

bool	line_has_valid_walls(
			char *line_to_check, int j, const char *original_line
			);

bool	is_neighbor_to_wall_char(char *map[], int i, int j)
{
	if (i != 0 && !line_has_valid_walls(map[i - 1], j, map[i]))
		return (false);
	if (!line_has_valid_walls(map[i], j, map[i]))
		return (false);
	if (map[i + 1] != NULL && !line_has_valid_walls(map[i + 1], j, map[i]))
		return (false);
	return (true);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:03:53 by shaas             #+#    #+#             */
/*   Updated: 2022/08/23 21:38:38 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


static int	tex_rgba(t_texture_calc *tex, mlx_texture_t *wall)
{
	int	pixel;

	pixel = (tex->texture_pixel[Y] * wall->width + tex->texture_pixel[X]) * 4;
	return (rgba(wall->pixels[pixel], wall->pixels[pixel + 1],
			wall->pixels[pixel + 2], wall->pixels[pixel + 3]));
}

static inline void	texture_calc(
	t_texture_calc *tex, t_raycasting_calc *cast, t_game *game)
{
	tex->wall_height = (int)(SCREENHEIGHT * WALLHEIGHT) / cast->wall_distance;
	tex->first_pixel = (SCREENHEIGHT * WALLHEIGHT / 2) - (tex->wall_height / 2);
	if (tex->first_pixel < 0)
		tex->first_pixel = 0;
	tex->last_pixel = (SCREENHEIGHT * WALLHEIGHT / 2) + (tex->wall_height / 2);
	if (tex->last_pixel >= (SCREENHEIGHT * WALLHEIGHT))
		tex->last_pixel = SCREENHEIGHT * WALLHEIGHT - 1;
	if (cast->hit_border == NO_SO)
		tex->wall_hitpoint = game->vectors.player_position[Y]
			+ (cast->wall_distance * cast->ray_vector[Y]);
	else
		tex->wall_hitpoint = game->vectors.player_position[X]
			+ (cast->wall_distance * cast->ray_vector[X]);
	tex->wall_hitpoint -= floor(tex->wall_hitpoint);
	tex->texture_pixel[X] = (int)(tex->wall_hitpoint
			* (double)game->wall[cast->wall_direction]->width);
	if ((cast->hit_border == NO_SO && cast->ray_vector[X] > 0)
		|| (cast->hit_border == WE_EA && cast->ray_vector[Y] < 0))
		tex->texture_pixel[X] = game->wall[cast->wall_direction]->width
			- tex->texture_pixel[X] - 1;
	tex->step = ((double)game->wall[cast->wall_direction]->height)
		/ tex->wall_height;
	tex->texture_position = (tex->first_pixel - (SCREENHEIGHT * WALLHEIGHT / 2)
			+ (tex->wall_height / 2)) * tex->step;
}

void	draw_wall(t_raycasting_calc *cast, t_game *game, int ray_iter)
{
	t_texture_calc	tex;

	texture_calc(&tex, cast, game);
	while (tex.first_pixel < tex.last_pixel)
	{
		tex.texture_pixel[Y] = (int)tex.texture_position;
		mlx_put_pixel(game->mlx_img, ray_iter, tex.first_pixel,
			tex_rgba(&tex, game->wall[cast->wall_direction]));
		tex.texture_position += tex.step;
		tex.first_pixel++;
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:55:36 by shaas             #+#    #+#             */
/*   Updated: 2022/08/23 21:32:52 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


static inline void	ray_calc(
	t_raycasting_calc *cast, t_game *game, int ray_iter)
{
	cast->camera_plane_part = (ray_iter * 2 / (double)SCREENWIDTH) - 1;
	cast->ray_vector[X] = game->vectors.player_direction[X]
		+ (game->vectors.camera_plane[X] * cast->camera_plane_part);
	cast->ray_vector[Y] = game->vectors.player_direction[Y]
		+ (game->vectors.camera_plane[Y] * cast->camera_plane_part);
	cast->tile[X] = (int)game->vectors.player_position[X];
	cast->tile[Y] = (int)game->vectors.player_position[Y];
	if (cast->ray_vector[X] == 0)
		cast->tile_border_distance[X] = INFINITY;
	else
		cast->tile_border_distance[X] = fabs(1 / cast->ray_vector[X]);
	if (cast->ray_vector[Y] == 0)
		cast->tile_border_distance[Y] = INFINITY;
	else
		cast->tile_border_distance[Y] = fabs(1 / cast->ray_vector[Y]);
}

static inline void	put_floor_ceiling(
	mlx_image_t *mlx_img, t_scene_description *scene_desc)
{
	t_square_data	square;

	square.mlx_img = mlx_img;
	square.length[X] = SCREENWIDTH;
	square.length[Y] = SCREENHEIGHT / 2;
	square.start_pixel[X] = 0;
	square.start_pixel[Y] = 0;
	square.color = scene_desc->ceiling_color;
	put_square(&square);
	square.start_pixel[Y] = SCREENHEIGHT / 2;
	square.color = scene_desc->floor_color;
	put_square(&square);
}

/* variable "ray_iter" is equal to "x" in 42docs tutorial */
void	raycasting_loop(void *bundle)
{
	static int					ray_iter;
	static t_raycasting_calc	cast;
	t_game						*game;
	t_scene_description			*scene_desc;

	game = ((t_bundle *)bundle)->game;
	scene_desc = ((t_bundle *)bundle)->scene_desc;
	put_floor_ceiling(game->mlx_img, scene_desc);
	ray_iter = 0;
	while (ray_iter < SCREENWIDTH)
	{
		ray_calc(&cast, game, ray_iter);
		init_wall_hit_calc(&cast, game);
		wall_hit_calc(&cast, scene_desc);
		draw_wall(&cast, game, ray_iter);
		ray_iter++;
	}
	check_movement(&(game->vectors), scene_desc->map_content, game->mlx_ptr);
	check_rotation(&(game->vectors), game->mlx_ptr);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_hit_calc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschmitt <tschmitt@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:54:34 by shaas             #+#    #+#             */
/*   Updated: 2022/08/23 21:26:52 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


void	wall_hit_calc_result(t_raycasting_calc *cast)
{
	if (cast->hit_border == NO_SO)
	{
		cast->wall_distance = cast->player_to_tile_border[X]
			- cast->tile_border_distance[X];
		cast->wall_direction = cast->potential_wall_direction[X];
	}
	else
	{
		cast->wall_distance = cast->player_to_tile_border[Y]
			- cast->tile_border_distance[Y];
		cast->wall_direction = cast->potential_wall_direction[Y];
	}
}

void	wall_hit_calc(t_raycasting_calc *cast, t_scene_description *scene_desc)
{
	while (true)
	{
		if (cast->player_to_tile_border[X] < cast->player_to_tile_border[Y])
		{
			cast->player_to_tile_border[X] += cast->tile_border_distance[X];
			cast->tile[X] += cast->direction[X];
			cast->hit_border = NO_SO;
		}
		else
		{
			cast->player_to_tile_border[Y] += cast->tile_border_distance[Y];
			cast->tile[Y] += cast->direction[Y];
			cast->hit_border = WE_EA;
		}
		if (scene_desc->map_content[cast->tile[Y]][cast->tile[X]] == '1')
			break ;
	}
	wall_hit_calc_result(cast);
}

static inline void	x_direction(t_raycasting_calc *cast, t_game *game)
{
	if (cast->ray_vector[X] < 0)
	{
		cast->direction[X] = -1;
		cast->player_to_tile_border[X] = (game->vectors.player_position[X]
				- cast->tile[X]) * cast->tile_border_distance[X];
		cast->potential_wall_direction[X] = WE;
	}
	else
	{
		cast->direction[X] = 1;
		cast->player_to_tile_border[X] = (cast->tile[X] + 1.0 \
			- game->vectors.player_position[X]) * cast->tile_border_distance[X];
		cast->potential_wall_direction[X] = EA;
	}
}

static inline void	y_direction(t_raycasting_calc *cast, t_game *game)
{
	if (cast->ray_vector[Y] < 0)
	{
		cast->direction[Y] = -1;
		cast->player_to_tile_border[Y] = (game->vectors.player_position[Y]
				- cast->tile[Y]) * cast->tile_border_distance[Y];
		cast->potential_wall_direction[Y] = NO;
	}
	else
	{
		cast->direction[Y] = 1;
		cast->player_to_tile_border[Y] = (cast->tile[Y] + 1.0 \
			- game->vectors.player_position[Y]) * cast->tile_border_distance[Y];
		cast->potential_wall_direction[Y] = SO;
	}
}

void	init_wall_hit_calc(t_raycasting_calc *cast, t_game *game)
{
	x_direction(cast, game);
	y_direction(cast, game);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 16:53:18 by tschmitt          #+#    #+#             */
/*   Updated: 2022/08/24 01:20:34 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


void	free_at_window_close(void *arg)
{
	t_bundle	*bundle;

	bundle = (t_bundle *)arg;
	ft_free_split(bundle->scene_desc->map_content);
	free(bundle->scene_desc->textures[0]);
	free(bundle->scene_desc->textures[1]);
	free(bundle->scene_desc->textures[2]);
	free(bundle->scene_desc->textures[3]);
	mlx_delete_image(bundle->game->mlx_ptr, bundle->game->mlx_img);
	mlx_delete_texture(bundle->game->wall[NO]);
	mlx_delete_texture(bundle->game->wall[SO]);
	mlx_delete_texture(bundle->game->wall[WE]);
	mlx_delete_texture(bundle->game->wall[EA]);
}

void	close_at_esc(mlx_key_data_t key_data, void *arg)
{
	t_bundle	*bundle;

	bundle = (t_bundle *)arg;
	if (key_data.key == MLX_KEY_ESCAPE)
	{
		free_at_window_close(arg);
		mlx_close_window(bundle->game->mlx_ptr);
	}
}

int	put_error_msg(char *msg)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	return (EXIT_FAILURE);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 19:57:21 by shaas             #+#    #+#             */
/*   Updated: 2022/08/23 21:28:06 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


void	put_square(t_square_data *square)
{
	int	iter[2];

	iter[Y] = 0;
	while (iter[Y] < square->length[Y]
		&& iter[Y] + square->start_pixel[Y] < (int)square->mlx_img->height)
	{
		iter[X] = 0;
		while (iter[X] < square->length[X]
			&& iter[X] + square->start_pixel[X] < (int)square->mlx_img->width)
		{
			mlx_put_pixel(square->mlx_img, square->start_pixel[X] + iter[X],
				square->start_pixel[Y] + iter[Y], square->color);
			iter[X]++;
		}
		iter[Y]++;
	}
}

void	paste_png(t_game *game, char *png)
{
	mlx_texture_t	*tex;
	int				image[2];
	int				iter;

	image[Y] = 0;
	iter = 0;
	tex = mlx_load_png(png);
	while (image[Y] < (int)tex->height)
	{
		image[X] = 0;
		while (image[X] < (int)tex->width)
		{
			mlx_put_pixel(game->mlx_img, image[X] + 300, image[Y] + 300,
				rgba(tex->pixels[iter], tex->pixels[iter + 1],
					tex->pixels[iter + 2], tex->pixels[iter + 3]));
			iter += 4;
			image[X]++;
		}
		image[Y]++;
	}
	mlx_delete_texture(tex);
}

void	paste_texture(mlx_texture_t *tex, t_game *game)
{
	int	image[2];
	int	iter;

	image[Y] = 0;
	iter = 0;
	while (image[Y] < (int)tex->height)
	{
		image[X] = 0;
		while (image[X] < (int)tex->width)
		{
			mlx_put_pixel(game->mlx_img, image[X] + 300, image[Y] + 300,
				rgba(tex->pixels[iter], tex->pixels[iter + 1],
					tex->pixels[iter + 2], tex->pixels[iter + 3]));
			iter += 4;
			image[X]++;
		}
		image[Y]++;
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgba.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/22 20:13:41 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}
