# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: epinaud <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/25 16:30:14 by epinaud           #+#    #+#              #
#    Updated: 2025/06/16 20:22:30 by epinaud          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

EXE = philo

CC = cc

CFLAGS = -Wall -Wextra -Werror -g3

#ASAN = -fsanitize=address -g

INCLUDES = -I. -Iincludes/ -Ilibft/includes

LDLIBS = -Llibft -lft -pthread

OBJ_DIR = .obj

SRCS = philosophers.c actions.c cleanup.c putlogs.c time.c

$(OBJ_DIR)/%.o : srcs/%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

all: ftbranch libft $(OBJ_DIR) $(EXE)

$(EXE) :
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

philo: $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

libft:
	@make -C libft

relibft:
	@make re -C libft

ftbranch:
	@git -C libft checkout main

.obj:
	@mkdir -p .obj

clean:
	@rm -f $(EXE)
	@make clean -C libft

fclean:  clean
	@rm -rf $(OBJ_DIR)
	@make fclean -C libft

re: ftbranch fclean relibft $(OBJ_DIR) $(EXE)

.PHONY:  all clean fclean re libft relibft
