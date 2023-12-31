# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/21 12:39:52 by diodos-s          #+#    #+#              #
#    Updated: 2024/01/02 14:37:20 by rumachad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC_BUILTIN = builtin
SRC_ENV = env
SRC_MINI = minishell
SRC_EXPANSION = expansion
SRC_PARSER = parser
SRC_EXEC = execve

SRC = 	$(wildcard $(SRC_BUILTIN)/*.c) \
		$(wildcard $(SRC_ENV)/*.c) \
		$(wildcard $(SRC_EXPANSION)/*.c) \
		$(wildcard $(SRC_PARSER)/*.c) \
		$(wildcard $(SRC_EXEC)/*.c) \
		$(wildcard *.c) \
		
OBJS = ${SRC:.c=.o}
CFLAGS = -Wall -Wextra -Werror -I include -fsanitize=address

${NAME}:	${OBJS}
			make -C libft
			ar rcs minishell.a $(OBJS)
			cc ${CFLAGS} minishell.a libft/libft.a -lreadline -o $@

%.o: %.c
	cc ${CFLAGS} -c -o $@ $<

all: ${NAME}

clean:
		make clean -C libft
		rm -f $(OBJS)

fclean: clean
		rm -f minishell.a minishell libft/libft.a

re: fclean all

val:
		make
		valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell

.SILENT:
