/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hotrace.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skarayil <skarayil@student.42kocaeli>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:26:41 by skarayil          #+#    #+#             */
/*   Updated: 2026/09/12 16:34:19 by skarayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "hotrace.h"

int	main(void)
{
	char	*input;
	t_table	*table;
    char	*cursor;

	input = read_all(0);
	if (!input)
		return (1);
	table = table_create(1024);
	if (!table)
	{
		free(input);
		return (1);
	}
	pars_pairs(table, input, &cursor);
	process_queries(table, cursor);
	table_free(table);
	free(input);
	return (0);
}
