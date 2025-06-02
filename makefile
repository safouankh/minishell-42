CC = cc
CFLAGS = -Wall -Wextra -Werror
READLINE_INC = -I$(HOME)/mylibs/readline/include
READLINE_LIB = -L$(HOME)/mylibs/readline/lib -lreadline
LIBFT = ./ft_libft/libft.a
NAME = minishell

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C ./ft_libft

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE_LIB) $(READLINE_INC) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(READLINE_INC) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C ./ft_libft clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C ./ft_libft fclean

re: fclean all

mk: all clean

.PHONY: all clean fclean re $(LIBFT)