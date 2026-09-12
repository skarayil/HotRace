/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hotrace.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skarayil <skarayil@student.42kocaeli>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 21:05:43 by skarayil          #+#    #+#             */
/*   Updated: 2026/09/12 18:44:13 by skarayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOTRACE_H
# define HOTRACE_H

# include <unistd.h>

typedef struct s_entry
{
	char			*key;
	char			*value;
	struct s_entry	*next;
}					t_entry;

typedef struct s_table
{
	t_entry			**buckets;
	size_t			size;
	size_t			count;
}					t_table;

typedef struct s_buffer
{
	char			*data;
	size_t			len;
	size_t			cap;
}					t_buffer;

int					ft_strcmp(const char *a, const char *b);
char				*ft_table_search(t_table *t, const char *key);
char				*ft_read_all(int fd);
t_table				*ft_table_create(size_t size);
size_t				ft_hash_str(const char *key);
void				ft_putstr_fd(const char *s, int fd);
void				ft_putchar_fd(char c, int fd);
void				ft_process_queries(t_table *table, char *cursor);
void				ft_parse_pairs(t_table *table, char *input, char **end);
void				ft_table_insert(t_table *t, char *key, char *value);
void				ft_table_resize(t_table *t);
void				ft_table_free(t_table *t);

#endif