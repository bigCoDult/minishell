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

INC = $(INC_DIR)/minishell.h
SRC = $(SRC_DIR)/env_array.c \
		$(SRC_DIR)/env_expand.c \
		$(SRC_DIR)/env_utils.c \
		$(SRC_DIR)/env_var.c \
		$(SRC_DIR)/env.c \
		$(SRC_DIR)/execute_built.c \
		$(SRC_DIR)/execute_built2.c \
		$(SRC_DIR)/execute_built3.c \
		$(SRC_DIR)/execute_built4.c \
		$(SRC_DIR)/execute_built5.c \
		$(SRC_DIR)/execute_built6.c \
		$(SRC_DIR)/execute_extern.c \
		$(SRC_DIR)/execute_extern2.c \
		$(SRC_DIR)/execute_extern3.c \
		$(SRC_DIR)/execute_here.c \
		$(SRC_DIR)/execute_here2.c \
		$(SRC_DIR)/execute_here2_1.c \
		$(SRC_DIR)/execute_here3.c \
		$(SRC_DIR)/execute_here4.c \
		$(SRC_DIR)/execute_here5.c \
		$(SRC_DIR)/execute_here6.c \
		$(SRC_DIR)/execute_path.c \
		$(SRC_DIR)/execute_path2.c \
		$(SRC_DIR)/execute_pipe.c \
		$(SRC_DIR)/execute_pipe1.c \
		$(SRC_DIR)/execute_pipe2.c \
		$(SRC_DIR)/execute_pipe3.c \
		$(SRC_DIR)/execute_pipe4.c \
		$(SRC_DIR)/execute_pipe5.c \
		$(SRC_DIR)/execute_pipe6.c \
		$(SRC_DIR)/execute_pipe7.c \
		$(SRC_DIR)/execute_pipe8.c \
		$(SRC_DIR)/execute_pipe9.c \
		$(SRC_DIR)/execute_utils.c \
		$(SRC_DIR)/execute.c \
		$(SRC_DIR)/execute2.c \
		$(SRC_DIR)/execute3.c \
		$(SRC_DIR)/exit_utils.c \
		$(SRC_DIR)/ft_2.c \
		$(SRC_DIR)/ft_3.c \
		$(SRC_DIR)/ft_4.c \
		$(SRC_DIR)/ft_5.c \
		$(SRC_DIR)/ft_6.c \
		$(SRC_DIR)/ft_7.c \
		$(SRC_DIR)/ft_8.c \
		$(SRC_DIR)/ft.c \
		$(SRC_DIR)/memory_utils.c \
		$(SRC_DIR)/memory_utils2.c \
		$(SRC_DIR)/memory.c \
		$(SRC_DIR)/parser_ast.c \
		$(SRC_DIR)/parser_pipe.c \
		$(SRC_DIR)/parser_simple.c \
		$(SRC_DIR)/parser_utils.c \
		$(SRC_DIR)/parser.c \
		$(SRC_DIR)/redir_utils.c \
		$(SRC_DIR)/redir_utils2.c \
		$(SRC_DIR)/redir_utils3.c \
		$(SRC_DIR)/redir_utils4.c \
		$(SRC_DIR)/redir_utils5.c \
		$(SRC_DIR)/restore.c \
		$(SRC_DIR)/signal.c \
		$(SRC_DIR)/signal2.c \
		$(SRC_DIR)/tokenizer_redir.c \
		$(SRC_DIR)/tokenizer_redir2.c \
		$(SRC_DIR)/tokenizer_utils.c \
		$(SRC_DIR)/tokenizer_utils2.c \
		$(SRC_DIR)/tokenizer_utils3.c \
		$(SRC_DIR)/tokenizer.c \
		$(SRC_DIR)/tokenizer2.c

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