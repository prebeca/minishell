NAME	=	minishell

C_DIR	=	src

O_DIR	=	obj

SRCS	=	${patsubst %, ${C_DIR}/%, ${C_FILES}}

C_FILES	=	main.c\
			minishell.c\
			signals_handler.c\
			prompt.c\
			process_input.c\
			tokenizer.c\
			tokenizer_get_token.c\
			tokenizer_utils.c\
			token_to_cmdlist.c\
			shell_expansions.c\
			variables_expansions.c\
			word_splitting.c\
			quote_removal.c\
			redirection.c\
			execute_builtin.c\
			execute_cmd.c\
			execute_cmd_search_path.c\
			utils.c\
			utils_str_tab.c\
			utils_pipe.c\
			builtin_env.c\
			builtin_pwd.c\
			builtin_env_utils.c\
			builtin_export.c\
			builtin_unset.c\
			builtin_export_print.c\
			builtin_echo.c\
			builtin_exit.c\
			builtin_cd.c

OBJS	=	${patsubst ${C_DIR}/%, ${O_DIR}/%, ${SRCS:.c=.o}}

CC		=	clang

CFLAGS	=	-I./inc/ -g -Wall -Wextra -Werror

LIBS	=	libft/libft.a

all:		${NAME}

${NAME}:	${OBJS}
			${MAKE} -C libft
			${CC} ${CFLAGS} ${OBJS} ${LIBS} -o ${NAME}

${O_DIR}/%.o : ${C_DIR}/%.c
			@mkdir -p obj
			${CC} ${CFLAGS} -c $< -o $@

.PHONY:		clean fclean re .c.o all

clean:
			rm -rf ${O_DIR}
			${MAKE}	-C libft clean

fclean:		clean
			rm -f ${NAME}
			${MAKE} -C libft fclean

re:			fclean all
