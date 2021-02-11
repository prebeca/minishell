/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 15:52:35 by user42            #+#    #+#             */
/*   Updated: 2021/02/11 14:12:00 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <errno.h>
# include <stdbool.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>

# include "libft.h"

# define METACHAR	" \t\n;|<>"
# define OPERATOR	";|<>"
# define BLANKS		" \t"

# define E_QUOTE	'\''
# define E_DQUOTE	'"'
# define E_ESCAPE	'\\'

# define O_LESS		'<'
# define O_GREAT	'>'
# define O_DGREAT	">>"
# define O_PIPE		'|'
# define O_SEMI		';'
# define O_DSEMI	";;"

typedef enum	e_token_tag
{
	T_WORD = 0,
	T_CTRL_OPE,
	T_REDIR_OPE,
	T_OPERAND
}				t_token_tag;

typedef struct	s_minishell
{
	bool		is_interactive;
	int			exit_status;
	char		*cwd;
	bool		exit;
	int			stdio_save[2];
}				t_minishell;

typedef struct	s_token
{
	char		*value;
	t_token_tag	tag;
}				t_token;

typedef struct	s_cmd
{
	t_list		*args;
	char		**args_tab;
	int			fd_output[2];
	int			fd_input[2];
	pid_t		pid;
	char		ctrl_ope;
	bool		is_async;
}				t_cmd;

char			**g_env;
bool			g_sig_int;

int				minishell(int argc, char **argv, char **envp);
int				prompt(t_minishell *shell_info);
int				print_prompt(void);
int				process_input(t_minishell *shell_info, char *input);
int				tokenizer(char *input, t_list **tokens);
int				token_to_cmdlist(t_list *tokens, t_list **cmd_list);
int				shell_expansions(t_cmd *cmd, t_minishell *shell_info);
int				redirection(t_cmd *cmd);
void			execute_cmd(t_minishell *minishell, char **argv);

/*
** built-in
*/
int				ft_cd(char **args);
int				ft_echo(char **argv);
int				ft_env(void);
int				ft_pwd(void);
int				ft_unset(char **args);
int				ft_exit(char **args, t_minishell *shell_info);
int				ft_export(char **argv);

/*
** signals handler
*/
void			sigquit_handler(int num);
void			sigint_handler_prompting(int num);
void			sigint_handler_executing(int num);

/*
** utils
*/
char			*ft_getenv(char	*name);
int				check_n_export(char *var, int *error);
int				ft_add_to_env(char *var);
int				ft_remove_from_env(char *name);
char			*ft_get_var_value(char *var);
char			*ft_get_var_name(char *var);
int				check_var_syntaxe(char *var);
char			**ft_tabdup(char **tab);
char			**ft_find_in_env(char *name);
int				table_len(char **tab);
void			free_tab(char **tab);
char			**lst_to_str_tab(t_list *lst);
t_list			*str_tab_to_lst(char **tab);
int				ft_strappend(char	**dst, char	*src);
void			ft_strsort(char **tab, int first, int last);
void			free_cmd(void *void_cmd);
int				pass_quote(char *input, int *end);
int				get_token(char *str, int *i, t_token **token);
t_token			*new_token(char	*new_value, t_token_tag new_tag);
char			**t_tokens_to_chartab(t_list *lst);
void			free_token(void *void_token);
int				word_splitting(t_list ***tokens);
int				variables_expansions(char **str, t_minishell *shell_info);
int				*quote_removal(char **str);
void			save_stdio(int stdio_save[2]);
void			reset_stdio(int save_stdio[2]);
int				put_error(char *func_name, char *error_cause,
char *error_desc, int errno_value);
int				pass_until_c(char *input, int *end, char c);
void			close_pipe(int pipe[2]);
int				pass_quote(char *input, int *end);
int				pass_d_quote(char *input, int *end);
char			*search_path(char **argv);
int				check_invalid_token(t_list *tokens);
char			*get_export_output_string(void);
void			search_n_exec(t_minishell *shell_info, char **argv);

#endif
