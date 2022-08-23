/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   music.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaas <shaas@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 01:15:19 by shaas             #+#    #+#             */
/*   Updated: 2022/08/24 01:23:35 by shaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	kill_music(void)
{
	system("kill 0");
}

void	init_music(char *start_texture)
{
	if (!MUSIC)
		return ;
	if (ft_strstr(start_texture, "flag"))
		system("zsh -c \"while true; do; afplay ./music/nyan-cat.mp3; done &\"");
	else if (ft_strstr(start_texture, "arcade"))
		system("zsh -c \"while true; do; afplay ./music/fun-disco.mp3; done &\"");
	else if (ft_strstr(start_texture, "cloud"))
		system("zsh -c \"while true; do; afplay ./music/peaceful-garden.mp3; done &;\"");
	else if (ft_strstr(start_texture, "ground"))
		system("zsh -c \"while true; do; afplay ./music/droplets-in-a-cave.mp3; done &;\"");
}
