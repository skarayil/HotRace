/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skarayil <skarayil@student.42kocaeli>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:17:38 by skarayil          #+#    #+#             */
/*   Updated: 2026/09/12 19:38:01 by skarayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"
#include <stdlib.h>

static t_entry	**ft_alloc_buckets(size_t size)
{
	t_entry	**buckets;
	size_t	i;

	buckets = malloc(sizeof(t_entry *) * size);
	if (!buckets)
		return (NULL);
	i = 0;
	while (i < size)
	{
		buckets[i] = NULL;
		i++;
	}
	return (buckets);
}

static void	ft_move_entries(t_table *t, t_entry **new_buckets, size_t new_size)
{
	t_entry	*e;
	t_entry	*next;
	size_t	i;
	size_t	j;

	i = 0;
	while (i < t->size)
	{
		e = t->buckets[i];
		while (e)
		{
			next = e->next;
			j = ft_hash_str(e->key) & (new_size - 1);
			e->next = new_buckets[j];
			new_buckets[j] = e;
			e = next;
		}
		i++;
	}
}

void	ft_table_resize(t_table *t)
{
	size_t	new_size;
	t_entry	**new_buckets;

	new_size = t->size * 2;
	new_buckets = ft_alloc_buckets(new_size);
	if (!new_buckets)
		return ;
	ft_move_entries(t, new_buckets, new_size);
	free(t->buckets);
	t->buckets = new_buckets;
	t->size = new_size;
}
