NAME	= minishell
SHELL	= /bin/sh
CC	= cc
CFLAGS	= -Wall -Wextra -Werror -g
LINK	= -lreadline -lncurses -L/opt/homebrew/opt/readline/lib
INC		= -I./ -I/opt/homebrew/opt/readline/include
RM	 = rm -f

SRCS	= main.c parse.c tokenizer.c free_utils.c \
		  expansion.c ft_strjoin.c allocate_argv.c builtin.c \
		  redirect_type.c redirect.c create_word.c token_utils.c \
		  expand_variables.c ft_split.c ft_itoa.c heredoc.c \
		  ft_substr.c env_management.c

OBJS	= $(SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LINK) -o $(NAME)

all: $(NAME)

clean:
		$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

debug:	all clean

re: fclean all

.PHONY: all clean fclean re
