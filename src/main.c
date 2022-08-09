/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:04:41 by shaas             #+#    #+#             */
/*   Updated: 2022/08/02 14:48:22 by tschmitt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	main(int argc, const char *argv[])
{
	t_scene_description	scene_description;

	if (argc != 2)
	{
		ft_putstr_fd("Error\nOne Argument Needed\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (parser(argv[1], &scene_description) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	//init_game()
	return (EXIT_SUCCESS);
}
