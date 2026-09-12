/* *********************************************************************** */
/*                                                                         */
/*                                                     :::      ::::::::   */
/* buffer.c                                          :+:      :+:    :+:   */
/*                                                 +:+ +:+         +:+     */
/* By: ksener <ksener@student.42kocaeli.com.tr   +#+  +:+       +#+        */
/*                                             +#+#+#+#+#+   +#+           */
/* Created: 2026/09/12 15:34:34 by ksener          #+#    #+#              */
/* Updated: 2026/09/12 15:34:37 by ksener          ###   ########.fr       */
/*                                                                         */
/* *********************************************************************** */

#include "hotrace.h"
#include <stdlib.h>

int	buffer_init(t_buffer *buffer, size_t cap)
{
	buffer->data = malloc(cap);
	if (buffer->data == NULL)
		return (0);
	buffer->len = 0;
	buffer->cap = cap;
	return (1);
}

int	buffer_grow(t_buffer *buffer)
{
	char	*new_data;
	size_t	new_cap;
	size_t	i;

	i = 0;
	new_cap = buffer->cap * 2;
	new_data = malloc(new_cap);
	if (!new_data)
		return (0);
	while (i < buffer->len)
	{
		new_data[i] = buffer->data[i];
		i++;
	}
	free(buffer->data);
	buffer->data = new_data;
	buffer->cap = new_cap;
	return (1);
}

int	buffer_append(t_buffer *buffer, const char *src, size_t n)
{
	size_t	i;

	while (buffer->len + n + 1 > buffer->cap)
	{
		if (!(buffer_grow(buffer)))
			return (0);
	}
	i = 0;
	while (i < n)
	{
		buffer->data[buffer->len + i] = src[i];
		i++;
	}
	buffer->len += n;
	buffer->data[buffer->len] = '\0';
	return (1);
}

char	*read_all(int fd)
{
	t_buffer buffer;
	char	chunk[4096];
	ssize_t	n;

	if (!(buffer_init(&buffer, 4096)))
		return (NULL);
	while ((n = read(fd, chunk, sizeof(chunk))) > 0)
	{
		if (!buffer_append(&buffer, chunk, (size_t)n))
		{
			free(buffer.data);
			return (NULL);
		}
	}
	if (n < 0)
	{
		free(buffer.data);
		return (NULL);
	}
	return (buffer.data);
}