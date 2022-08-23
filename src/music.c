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

#include "../inc/cub3d.h"

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
