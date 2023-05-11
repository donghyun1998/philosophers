# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/29 00:02:12 by donghyk2          #+#    #+#              #
#    Updated: 2023/05/11 12:23:38 by donghyk2         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= philo

CC				= @cc
CFLAGS			= -Wall -Werror -Wextra -pthread

SRC				= main.c utils.c

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

OBJS			= $(SRC:.c=.o)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	@rm -f $(OBJS)

fclean:
	@make clean
	@rm -f $(NAME)

re:
	@make fclean
	@make all

.PHONY: all clean fclean re
