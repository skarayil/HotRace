/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hotrace.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skarayil <skarayil@student.42kocaeli>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:26:41 by skarayil          #+#    #+#             */
/*   Updated: 2026/09/12 22:34:48 by skarayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"
#include <stdlib.h>

int	main(void)
{
	char	*input;
	t_table	*table;
	char	*cursor;

	input = ft_read_all(0);
	if (!input)
		return (1);
	table = ft_table_create(262144);
	if (!table)
	{
		free(input);
		return (1);
	}
	cursor = NULL;
	ft_parse_pairs(table, input, &cursor);
	ft_process_queries(table, cursor);
	ft_table_free(table);
	free(input);
	return (0);
}
