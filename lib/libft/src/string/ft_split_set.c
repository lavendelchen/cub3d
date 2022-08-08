/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_set.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toni <toni@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:20:06 by tschmitt          #+#    #+#             */
/*   Updated: 2021/10/30 16:16:03 by toni             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_arr_len(char *str, char *set)
{
	char	*trimmed_str;
	int		arr_len;
	int		i;

	trimmed_str = ft_strtrim(str, set);
	if (trimmed_str == NULL)
		return (-1);
	arr_len = 0;
	i = 0;
	if (ft_strchr(set, trimmed_str[i]) == NULL)
		arr_len++;
	while (trimmed_str[i])
	{
		if (ft_strchr(set, trimmed_str[i]) != NULL)
			arr_len++;
		i++;
	}
	free(trimmed_str);
	return (arr_len);
}

static size_t	get_element_size(char *str, char *set)
{
	size_t	size;

	size = 0;
	while (*str && ft_strchr(set, *str) == NULL)
	{
		size++;
		str++;
	}
	return (size);
}

static char	**fuck_norm_tbh(char *str, char *set, char *arr[], int *iter)
{
	int	arr_len;

	arr_len = get_arr_len(str, set);
	iter[0] = 0;
	while (iter[0] < arr_len && *str)
	{
		while (*str && ft_strchr(set, *str) != NULL)
			str++;
		if (*str == '\0')
			break ;
		arr[iter[0]] = ft_calloc(get_element_size(str, set) + 1, \
		sizeof(*arr[iter[0]]));
		if (arr[iter[0]] == NULL)
			return (ft_free_split(arr));
		iter[1] = 0;
		while (*str && ft_strchr(set, *str) == NULL)
			arr[iter[0]][iter[1]++] = *str++;
		iter[0]++;
	}
	return (arr);
}

char	**ft_split_set(char *str, char *set)
{
	char	**arr;
	int		arr_len;
	int		iter[2];

	if (str == NULL || set == NULL)
		return (NULL);
	arr_len = get_arr_len(str, set);
	arr = ft_calloc(arr_len + 1, sizeof(*arr));
	if (arr == NULL)
		return (NULL);
	iter[0] = 0;
	iter[1] = 0;
	return (fuck_norm_tbh(str, set, arr, iter));
}
