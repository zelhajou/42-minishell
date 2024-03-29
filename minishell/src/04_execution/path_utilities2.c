/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utilities2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beddinao <beddinao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:03:32 by beddinao          #+#    #+#             */
/*   Updated: 2024/03/10 09:42:59 by beddinao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_substr_index(char **haystack, char *needle, int n_length)
{
	int		a;
	int		b;

	a = 0;
	while (haystack[a] != 0)
	{
		if (haystack[a][0] == needle[0])
		{
			b = 0;
			while (haystack[a][b]
				&& haystack[a][b] == needle[b])
			{
				if (b == n_length - 1)
					return (a);
				b++;
			}
		}
		a++;
	}
	return (-1);
}

int	is_path_accessible(char *path, int mode)
{
	if (access(path, mode) < 0)
		return (0);
	return (1);
}

void	increment_path_index(char *env_var, int *indx_s, int *a)
{
	if (indx_s[3])
	{
		indx_s[1] += 1;
		*a = indx_s[1];
		while (env_var[indx_s[1]] != '\0'
			&& env_var[indx_s[1]] != ':')
			indx_s[1] += 1;
	}
	else
		*a = indx_s[1];
}

int	count_substrings(char *str, char del)
{
	int			a;
	int			b;
	int			res;

	a = 0;
	b = 1;
	res = 0;
	while (str && str[a])
	{
		if (str[a] != del)
		{
			if (b)
				res += 1;
			b = 0;
		}
		else
			b = 1;
		a += 1;
	}
	return (res);
}
