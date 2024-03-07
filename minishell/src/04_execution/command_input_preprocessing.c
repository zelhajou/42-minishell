/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_input_preprocessing.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelhajou <zelhajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:12:09 by beddinao          #+#    #+#             */
/*   Updated: 2024/03/07 08:38:07 by beddinao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**merge_command_args(char **f_args, char **_cmd_)
{
	int				a;
	char			**new_args;

	if (!f_args)
		return (NULL);
	a = 1;
	while (_cmd_[a])
		a++;
	new_args = malloc((a + 1) * sizeof(char *));
	if (!new_args)
		return (NULL);
	a = 0;
	new_args[a] = strcopy(f_args[a]);
	while (_cmd_[++a])
		new_args[a] = strcopy(_cmd_[a]);
	new_args[a] = 0;
	free_string_array(f_args);
	return (new_args);
}

void	switch_fds_identifier(int *_piped, int index, int index_2)
{
	if (_piped[index])
		close(_piped[index_2]);
	_piped[index] = 1;
}
