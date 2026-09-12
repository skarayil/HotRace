/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skarayil <skarayil@student.42kocaeli>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 17:40:41 by skarayil          #+#    #+#             */
/*   Updated: 2026/09/12 18:42:32 by skarayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"

static char	*ft_next_line(char **cursor)
{
	char	*line;
	char	*p;

	if (!cursor || !*cursor || !**cursor)
		return (NULL);
	p = *cursor;
	line = p;
	while (*p && *p != '\n')
		p++;
	if (*p == '\n')
	{
		*p = '\0';
		p++;
	}
	*cursor = p;
	return (line);
}

void	ft_parse_pairs(t_table *table, char *input, char **end)
{
	char	*cursor;
	char	*key;
	char	*value;

	cursor = input;
	while (1)
	{
		key = ft_next_line(&cursor);
		if (!key || *key == '\0')
			break ;
		value = ft_next_line(&cursor);
		if (!value)
			break ;
		ft_table_insert(table, key, value);
	}
	if (end)
		*end = cursor;
}

void	ft_process_queries(t_table *table, char *cursor)
{
	char	*query;
	char	*res;

	while (1)
	{
		query = ft_next_line(&cursor);
		if (!query)
			break ;
		if (*query == '\0')
			continue ;
		res = ft_table_search(table, query);
		if (res)
		{
			ft_putstr_fd(res, 1);
			ft_putchar_fd('\n', 1);
		}
		else
		{
			ft_putstr_fd(query, 1);
			ft_putstr_fd(": Not found.\n", 1);
		}
	}
}
