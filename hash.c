/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skarayil <skarayil@student.42kocaeli>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 17:40:49 by skarayil          #+#    #+#             */
/*   Updated: 2026/09/12 22:50:01 by skarayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"
#include <stdlib.h>

static unsigned long	ft_load_mix(const char *p)
{
	unsigned long	v;

	__builtin_memcpy(&v, p, 8);
	v *= 0x9E3779B97F4A7C15UL;
	return ((v << 31) | (v >> 33));
}

size_t	ft_hash_str(const char *key)
{
	size_t			len;
	size_t			i;
	unsigned long	h;

	len = ft_strlen(key);
	i = 0;
	h = 0x9E3779B97F4A7C15UL + len;
	while (i + 8 <= len)
	{
		h ^= ft_load_mix(key + i);
		h = (h << 27) | (h >> 37);
		h = h * 0xBF58476D1CE4E5B9UL + 0x94D049BB133111EBUL;
		i += 8;
	}
	while (i < len)
	{
		h ^= (unsigned char)key[i];
		h *= 0x100000001B3UL;
		i++;
	}
	h ^= h >> 33;
	h *= 0xFF51AFD7ED558CCDUL;
	h ^= h >> 29;
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
