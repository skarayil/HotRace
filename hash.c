/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skarayil <skarayil@student.42kocaeli>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 17:40:49 by skarayil          #+#    #+#             */
/*   Updated: 2026/09/12 19:36:56 by skarayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"
#include <stdlib.h>

size_t	ft_hash_str(const char *key)
{
	size_t	h;
	size_t	i;

	h = (size_t)14695981039346656037ULL;
	i = 0;
	while (key[i])
	{
		h ^= (unsigned char)key[i];
		h *= (size_t)1099511628211ULL;
		i++;
	}
	return (h);
}

t_table	*ft_table_create(size_t size)
{
	t_table	*t;
	size_t	i;

	t = malloc(sizeof(t_table));
	if (!t)
		return (NULL);
	t->buckets = malloc(sizeof(t_entry *) * size);
	if (!t->buckets)
	{
		free(t);
		return (NULL);
	}
	i = 0;
	while (i < size)
	{
		t->buckets[i] = NULL;
		i++;
	}
	t->size = size;
	t->count = 0;
	return (t);
}

char	*ft_table_search(t_table *t, const char *key)
{
	size_t	j;
	t_entry	*e;

	if (!t || !key)
		return (NULL);
	j = ft_hash_str(key) & (t->size - 1);
	e = t->buckets[j];
	while (e)
	{
		if (ft_strcmp(e->key, key) == 0)
			return (e->value);
		e = e->next;
	}
	return (NULL);
}

void	ft_table_free(t_table *t)
{
	size_t	i;
	t_entry	*e;
	t_entry	*next;

	if (!t)
		return ;
	i = 0;
	while (i < t->size)
	{
		e = t->buckets[i];
		while (e)
		{
			next = e->next;
			free(e);
			e = next;
		}
		i++;
	}
	free(t->buckets);
	free(t);
}
