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

#include "../../../inc/cub3d.h"
#include "../../../inc/parser.h"

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
