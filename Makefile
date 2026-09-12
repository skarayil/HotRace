NAME		= hotrace
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -O2
SRCS		= hotrace.c hash.c libft.c parser.c buffer.c resize.c insert.c
OBJS		= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
OBJ_DIR		= obj
HEADERS		= hotrace.h

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re