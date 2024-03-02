/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelhajou <zelhajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:56:51 by zelhajou          #+#    #+#             */
/*   Updated: 2024/03/02 17:28:26 by zelhajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_error_checker(const char *input)
{
	if (has_unclosed_quotes(input))
	{
		ft_putstr_fd("Syntax error: unclosed quote\n", STDERR_FILENO);
		return (1);
	}
	if (has_invalid_redirections(input))
	{
		ft_putstr_fd("Syntax error: invalid redirection\n", STDERR_FILENO);
		return (1);
	}
	if (has_misplaced_operators(input))
	{
		ft_putstr_fd("Syntax error: misplaced operator\n", STDERR_FILENO);
		return (1);
	}
	if (has_logical_operators(input))
	{
		ft_putstr_fd("Error: Logical operators '&&' and '||' \
			are not supported.\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	has_unclosed_quotes(const char *input)
{
	char	quote_type;

	quote_type = 0;
	while (*input)
	{
		if (*input == '\'' || *input == '\"')
		{
			if (quote_type == *input)
				quote_type = 0;
			else if (!quote_type)
				quote_type = *input;
		}
		input++;
	}
	return (quote_type != 0);
}

int	has_invalid_redirections(const char *input)
{
	int	s_q_count;
	int	d_q_count;

	s_q_count = 0;
	d_q_count = 0;
	while (*input)
	{
		update_quote_counts(*input, &s_q_count, &d_q_count);
		if ((!(s_q_count % 2) && !(d_q_count % 2))
			&& (*input == '>' || *input == '<'))
		{
			if (is_invalid_operator(&input))
				return (1);
		}
		else
			input++;
	}
	return (0);
}

int	has_misplaced_operators(const char *input)
{
	int	expect_command_next;
	int	s_q_count;
	int	d_q_count;

	s_q_count = 0;
	d_q_count = 0;
	expect_command_next = 0;
	if (*input == '|' || *input == '&')
		return (1);
	while (*input)
	{
		update_quote_counts(*input, &s_q_count, &d_q_count);
		if (*input == '|' && !(s_q_count % 2) && !(d_q_count % 2))
		{
			if (expect_command_next)
				return (1);
			expect_command_next = 1;
		}
		else if (!ft_isspace(*input))
			expect_command_next = 0;
		input++;
	}
	if (expect_command_next)
		return (1);
	return (0);
}

int	has_logical_operators(const char *input)
{
	int							s_q_count;
	int							d_q_count;

	s_q_count = 0;
	d_q_count = 0;
	while (*input)
	{
		update_quote_counts(*input, &s_q_count, &d_q_count);
		if (!(d_q_count % 2) && !(s_q_count % 2)
			&& ((*input == '&' && *(input + 1) == '&')
				|| (*input == '|' && *(input + 1) == '|')))
			return (1);
		input++;
	}
	return (0);
}
