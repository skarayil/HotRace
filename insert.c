/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skarayil <skarayil@student.42kocaeli>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:21:24 by skarayil          #+#    #+#             */
/*   Updated: 2026/09/12 18:35:54 by skarayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"

static t_entry	*ft_find_entry(t_table *t, const char *key, size_t j)
{
	t_entry	*e;

	e = t->buckets[j];
	while (e)
	{
		if (ft_strcmp(e->key, key) == 0)
			return (e);
		e = e->next;
	}
	return (NULL);
}

static t_entry	*ft_create_entry(char *key, char *value, t_entry *next)
{
	t_entry	*e;

	e = malloc(sizeof(t_entry));
	if (!e)
		return (NULL);
	e->key = key;
	e->value = value;
	e->next = next;
	return (e);
}

void	ft_table_insert(t_table *t, char *key, char *value)
{
	size_t	j;
	t_entry	*e;

	if (!t || !key || !value)
		return ;
	j = ft_hash_str(key) & (t->size - 1);
	e = ft_find_entry(t, key, j);
	if (e)
	{
		e->value = value;
		return ;
	}
	e = ft_create_entry(key, value, t->buckets[j]);
	if (!e)
		return ;
	t->buckets[j] = e;
	t->count++;
	if (t->count > t->size * 3 / 4)
		ft_table_resize(t);
}
