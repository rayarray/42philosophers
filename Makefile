# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rleskine <rleskine@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/15 16:29:22 by rleskine          #+#    #+#              #
#    Updated: 2023/08/27 08:15:28 by rleskine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	philo
#BONUS		=	checker
SRC			=	philo_utils1.c philo_utils2.c philo_utils3.c log_utils1.c log_utils2.c
OBJ			=	$(SRC:.c=.o)
CFLAGS		=	-Wall -Wextra -Werror -pthread -O0
DFLAGS		=	-g
#-fsanitize=thread
# remove fsanitize to make leaks work
NPROCS 		=	$(shell sysctl hw.ncpu | grep -o '[0-9]\+')

CC =	cc

all: $(NAME)

$(NAME): %: %.c $(OBJ)
	@$(CC) $(CFLAGS) $(DFLAGS) -o $@ $< $(OBJ)
	@echo "\033[32m[$(NAME)]: compiled\033[0m"

$(OBJ): %.o: %.c
	@$(CC) $(CFLAGS) $(DFLAGS) -c -o $@ $<
	@echo "\033[2K[$(NAME)]: Compilation of $< \033[A"

#bonus: $(BONUS)

#$(BONUS): %: %.c $(OBJ)
#	$(CC) $(CFLAGS) $(INCLUDE) $(LIBFLAGS) -o $@ $< $(OBJ)

clean:
	@rm -rf $(OBJ)
	@echo "\033[31mObjects files \033[1;31m$(OBJ)\033[1;0m\033[31m removed.\033[0m"

fclean: clean
	@/bin/rm -f $(NAME) $(BONUS)
	@echo "\033[31mBin \033[1;31m$(NAME)\033[1;0m\033[31m removed.\033[0m"

re: fclean all

#.SILENT:
