NAME = minishell

CC = gcc
RM = rm -rf

CFLAGS = -Wall -Wextra -Werror -g -O0
LDFLAGS = -lreadline

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

MAIN_SRC = main.c
MAIN_OBJ = $(OBJ_DIR)/main.o

SRC = $(wildcard $(SRC_DIR)/*.c) 
INC = $(wildcard $(INC_DIR)/*.h)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(MAIN_OBJ) $(OBJ)
	$(CC) $(MAIN_OBJ) $(OBJ) $(LDFLAGS) -o $(NAME)

$(MAIN_OBJ): $(MAIN_SRC) $(INC)
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC)
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re