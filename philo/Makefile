# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: epinaud <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/25 16:30:14 by epinaud           #+#    #+#              #
#    Updated: 2025/06/27 09:08:56 by epinaud          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

EXE = philo

CC = cc

CFLAGS = -Wall -Wextra -Werror -g3

INCLUDES = -I. -Iincludes/

LDLIBS = -pthread

OBJ_DIR = .obj

SRCS = philosophers.c dinner.c actions.c lib_utils.c putlogs.c time.c

$(OBJ_DIR)/%.o : srcs/%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

all: $(OBJ_DIR) $(EXE)

$(EXE) :
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

philo: $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

.obj:
	@mkdir -p .obj

clean:
	@rm -f $(EXE)

fclean:  clean
	@rm -rf $(OBJ_DIR)

re: fclean $(OBJ_DIR) $(EXE)

.PHONY:  all clean fclean re
