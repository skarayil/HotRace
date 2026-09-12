# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: skarayil <skarayil@student.42kocaeli>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/11 21:05:30 by skarayil          #+#    #+#              #
#    Updated: 2026/09/11 21:07:32 by skarayil         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = hotrace
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = hotrace.c hash.c libft.c parser.c buffer.c
OBJS = $(SRCS:.c=.o)
HEADERS = hotrace.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re