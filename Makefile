NAME = minishell
SRCS = allocate_argv.c create_word.c expansion.c redirect.c\
	   ft_strjoin.c	main.c parse.c token_utils.c tokenizer.c redirect_type.c
OBJS = $(SRCS:%.c=%.o)
CFLAGS =  -Wall -Wextra -Werror -Wno-unused-but-set-variable #-fsanitize=address
LINK = -lreadline -L/opt/homebrew/opt/readline/lib
INC = -I./minishell -I/opt/homebrew/opt/readline/include
CC = cc
RM = rm -f

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(LINK) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

all : $(NAME)

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY :all clean fclean
