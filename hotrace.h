/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hotrace.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skarayil <skarayil@student.42kocaeli>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 21:05:43 by skarayil          #+#    #+#             */
/*   Updated: 2026/09/11 21:07:17 by skarayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOTRACE_H
# define HOTRACE_H

# include <stdlib.h>
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

/* libft.c */
size_t				ft_strlen(const char *s);
int					ft_strcmp(const char *a, const char *b);
void				ft_putstr_fd(const char *s, int fd);
void				ft_putchar_fd(char c, int fd);

/* read */


/* hash.c */


/* hotrace.c */


#endif