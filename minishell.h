/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 18:38:59 by mawako            #+#    #+#             */
/*   Updated: 2025/04/19 18:11:28 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define SINGLE_QUOTE_CHAR '\''
# define DUOBLE_QUOTE_CHAR '\"'
# define ERROR_TOKENIZE 3
# define STDOUT 1
# define STDIN 0
# define STDERR 2

# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "env_management.h"

typedef enum e_redir_type
{
	RD_IN,
	RD_OUT,
	RD_APPEND_OUT,
	RD_OUT_ERR,
	RD_SP,
	RD_APPEND_IN,
	RD_HEREDOC,
}	t_redir_type;

typedef struct s_redirect
{
	t_redir_type		type;
	char				*word;
	int					quoted;
	int					heredoc_expand;
	char				*heredoc_filename;
	struct s_redirect	*next;
	int					filefd;
	int					targetfd;
	int					stashed_targetfd;
	int					targetfd2;
	int					stashed_targetfd2;
}	t_redirect;

typedef enum e_token_kind
{
	TK_WORD,
	TK_RESERVED,
	TK_OP,
	TK_EOF
}	t_token_kind;

typedef struct s_token
{
	char			*word;
	t_token_kind	kind;
	struct s_token	*next;
}	t_token;

typedef enum e_node_kind
{
	ND_SIMPLE_CMD,
	ND_SUBSHELL
}	t_node_kind;

typedef struct s_node
{
	t_token				*args;
	t_node_kind			kind;
	struct s_node		*next;
	struct s_redirect	*redirects;
	char				*separator;
	int					targetfd;
	int					targetfd2;
	t_token				*filename;
	int					filefd;
	int					stashed_targetfd;
	int					stashed_targetfd2;
	struct s_node		*child;
}	t_node;

extern pid_t	g_last_bg_pid;
extern char	**environ;

void		fatal_error(const char *msg) __attribute__((noreturn));
int			is_builtin(char *cmd);
int			exec_builtin(char **argv, t_env *env);
char		*search_path(const char *filename);
void		interpret(char *line, int *status, t_env *env);
char		**allocate_argv(t_token *tok, size_t size);
char		**create_argv(t_token *tok);
int			exec_cmd(t_node *node, t_env *env);
int			exec(t_node *node, t_env *env);
t_token		*tokenize(char *line);
void		tokenize_error(const char *location, char **rest, char *line);
int			is_blank(char c);
int			consume_blank(char **rest, char *line);
int			startswith(const char *s, const char *keyword);
int			check_redirect(const char *s);
int			is_metacharacter(char c);
int			is_word(const char *s);
t_token		*new_token(char *word, t_token_kind kind);
t_token		*operator_func(char **rest, char *line);
t_token		*word_func(char **rest, char *line);
void		expansion(t_node *node, t_env *env);
void		quote_removal(t_node *node, t_env *env);
void		remove_quote(t_token *tok, t_env *env);
char		*find_env(char *p);
char		*ft_strjoin(const char *s1, const char *s2);
t_node		*new_node(t_node_kind kind);
t_token		*tokdup(t_token *tok);
void		append_tok(t_token **tokens, t_token *tok);
t_node		*parse(t_token *tok);
t_redirect	*redirect_type(t_token **rest, t_token *tok, char *type);
t_redirect	*redirect_type2(t_token **rest, t_token *tok, char *type);
void		free_node(t_node *node);
void		free_token(t_token *tok);
void		free_redirects(t_redirect *redir);
void		free_argv(char **argv);
void		free_strs(char **strs);
char		**ft_split(char const *s, char c);
int			read_heredoc(const char *delimiter, int expand, char *heredoc_file, t_env *env);
char		*expand_variables(const char *str, t_env *env);
char		*ft_itoa(int n);
char		*ft_substr(char const *s, unsigned int start, size_t len);
int			open_redir_file(t_redirect *redir);
void		do_redirect(t_redirect *redir);
void		reset_redirect(t_redirect *redir);
void		quote_removal_redirects(t_redirect *redir);
int			setup_heredoc(t_node *node, t_env *env);
int			is_var_char(char c);
void		handle_dollar(const char *str, int *i, char **result, t_env *env);
void		append_char(char **result, char c);
char		*ft_strjoin_char_and_free(char *s, char c);
char		*ft_strjoin_and_free(char *s1, char *s2);
int			is_var_char(char c);
char		*expand_exit_status(t_env *env);
char		*get_shell_pid_str(void);
char		*get_env_value(const char *name, t_env *env);
void		sigint_handler(int);
void		sigchld_handler(int sig);
void		flush_stdin(void);
int			count_pl_nodes(t_node *head);
int			**setup_pipes(int n);
void		cleanup_pipes(int **pipes, int n);
void		setup_dup(int i, int n, int **pipes);
void		close_all_pipes(int n, int **pipes);
void		execute_command(t_node *node, t_env *env);
void		setup_pipe_child(t_node *node, int i, int n, int **pipes, t_env *env);
pid_t		*setup_pipe_children(t_node *head, int n, int **pipes, t_env *env);
int			wait_pl_children(pid_t *pids, int n);
int			exec_pl(t_node *head, t_env *env);
int			exec_sh_c(char **argv, t_env *env);
int			exec_tree(t_node *node, t_env *env);
int			exec_background(t_node *node, t_env *env);
int			handle_subshell(t_node **node_ptr, t_env *env);
int			handle_pl(t_node **node_ptr, t_env *env);
int			handle_bg(t_node **node_ptr, t_env *env);
int			handle_logical_separator(t_node **node_ptr, int status);

#endif