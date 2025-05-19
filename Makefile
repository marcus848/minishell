NAME		= minishell
CC		= cc
CFLAGS		= -Wall -Wextra -Werror -g
RM		= rm -rf

SRC_DIR		= src
OBJ_DIR		= obj
INCLUDE		= -Iinclude
LIBS		= -lreadline
LIBFT_DIR	= libft
LIBFT_LIB	= ${LIBFT_DIR}/libft.a

CLEAN_DIR	= ${SRC_DIR}/clean
DEBUG_DIR	= ${SRC_DIR}/debug
EXPANSION_DIR	= ${SRC_DIR}/expansion
TOKENIZER_DIR	= ${SRC_DIR}/tokenizer
AST_DIR		= ${SRC_DIR}/ast
SYNTAX_DIR	= ${SRC_DIR}/syntax
EXEC_DIR	= ${SRC_DIR}/executor
ENV_DIR		= ${SRC_DIR}/env
BUILTIN_DIR	= ${SRC_DIR}/builtin

SRCS		= $(SRC_DIR)/main.c \
		  ${CLEAN_DIR}/clean.c \
		  ${CLEAN_DIR}/clean_ast.c \
		  ${TOKENIZER_DIR}/tokenizer.c \
		  ${TOKENIZER_DIR}/tokenizer_handle_operators.c \
		  ${TOKENIZER_DIR}/tokenizer_utils.c \
		  ${TOKENIZER_DIR}/token_list.c \
		  ${DEBUG_DIR}/debug_functions.c \
		  ${DEBUG_DIR}/debug_command.c \
		  ${DEBUG_DIR}/debug_ast.c \
		  ${EXPANSION_DIR}/expansion.c \
		  ${EXPANSION_DIR}/expansion_utils.c \
		  ${EXPANSION_DIR}/expansion_types.c \
		  ${AST_DIR}/ast_utils.c \
		  ${AST_DIR}/ast.c \
		  ${AST_DIR}/commands.c \
		  ${SYNTAX_DIR}/syntax_analysis.c \
		  ${SYNTAX_DIR}/syntax_analysis_utils.c \
		  ${SYNTAX_DIR}/syntax_errors.c \
		  ${EXEC_DIR}/executor.c \
		  ${EXEC_DIR}/executor_fds.c \
		  ${EXEC_DIR}/executor_redirs.c \
		  ${EXEC_DIR}/executor_utils.c \
		  ${EXEC_DIR}/executor_status.c \
		  ${EXEC_DIR}/executor_dispatch.c \
		  ${EXEC_DIR}/executor_builtin.c \
		  ${EXEC_DIR}/executor_path_utils.c \
		  ${ENV_DIR}/env_init.c \
		  ${ENV_DIR}/env_utils.c \
		  ${BUILTIN_DIR}/builtin_exit.c \
		  ${BUILTIN_DIR}/builtin_pwd.c \

OBJS		= ${SRCS:${SRC_DIR}/%.c=${OBJ_DIR}/%.o}

# Colors
GREEN	= \033[0;32m
RESET	= \033[0m

all:		${NAME}

${NAME}:	${OBJS} ${LIBFT_LIB} 
			@${CC} ${CFLAGS} ${OBJS} ${LIBFT_LIB} ${LIBS} -o ${NAME}
			@echo "$(GREEN)Done!$(RESET)"

${OBJ_DIR}/%.o:	${SRC_DIR}/%.c
			@mkdir -p ${@D}
			@${CC} ${CFLAGS} ${INCLUDE} -I ${LIBFT_DIR} -c $< -o $@

${LIBFT_LIB}:
			@echo "Building libft..."
			@make -C ${LIBFT_DIR} --no-print-directory

leak:
			valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=readline.supp ./${NAME}

clean:
			@echo "Cleaning object files..."
			@${RM} ${OBJ_DIR}
			@echo "Cleaning libft objects..."
			@make -s -C ${LIBFT_DIR} clean --no-print-directory

fclean:		clean
			@echo "Removing binary and libraries..."
			@${RM} ${NAME}
			@make -s -C ${LIBFT_DIR} fclean --no-print-directory

re:		fclean all

.PHONY:		all clean fclean re

