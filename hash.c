/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skarayil <skarayil@student.42kocaeli>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:39:20 by skarayil          #+#    #+#             */
/*   Updated: 2026/09/12 16:39:22 by skarayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"

static size_t	hash_str(const char *key)
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

t_table	*table_create(size_t size)
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

static void	table_resize(t_table *t)
{
	size_t	new_size;
	t_entry	**new_buckets;
	t_entry	*e;
	t_entry	*next;
	size_t	i;

	new_size = t->size * 2;
	new_buckets = malloc(sizeof(t_entry *) * new_size);
	if (!new_buckets)
		return ;
	i = 0;
	while (i < new_size)
		new_buckets[i++] = NULL;
	i = 0;
	while (i < t->size)
	{
		e = t->buckets[i];
		while (e)
		{
			next = e->next;
			size_t idx = hash_str(e->key) & (new_size - 1);
			e->next = new_buckets[idx];
			new_buckets[idx] = e;
			e = next;
		}
		i++;
	}
	free(t->buckets);
	t->buckets = new_buckets;
	t->size = new_size;
}

void	table_insert(t_table *t, char *key, char *value)
{
	size_t	idx;
	t_entry	*e;

	if (!t || !key || !value)
		return ;
	idx = hash_str(key) & (t->size - 1);
	e = t->buckets[idx];
	while (e)
	{
		if (ft_strcmp(e->key, key) == 0)
		{
			e->value = value;
			return ;
		}
		e = e->next;
	}
	e = malloc(sizeof(t_entry));
	if (!e)
		return ;
	e->key = key;
	e->value = value;
	e->next = t->buckets[idx];
	t->buckets[idx] = e;
	t->count++;
	if (t->count > t->size * 3 / 4)
		table_resize(t);
}

char	*table_search(t_table *t, const char *key)
{
	size_t	idx;
	t_entry	*e;

	if (!t || !key)
		return (NULL);
	idx = hash_str(key) & (t->size - 1);
	e = t->buckets[idx];
	while (e)
	{
		if (ft_strcmp(e->key, key) == 0)
			return (e->value);
		e = e->next;
	}
	return (NULL);
}

void	table_free(t_table *t)
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
