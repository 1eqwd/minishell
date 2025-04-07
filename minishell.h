/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 18:38:59 by mawako            #+#    #+#             */
/*   Updated: 2025/04/07 13:54:00 by shuu             ###   ########.fr       */
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
# define R_HEREDOC 3

# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "env_management.h"

typedef struct s_redirect
{
	int					type;
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
	TK_EOF,
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
	ND_REDIR_OUT,
	ND_REDIR_IN,
	ND_REDIR_OUT_ERR,
	ND_REDIR_SP,
	ND_REDIR_APPEND_OUT,
	ND_REDIR_APPEND_IN,
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
}	t_node;

extern pid_t	g_last_bg_pid;

void		fatal_error(const char *msg) __attribute__((noreturn));
int			is_builtin(char *cmd);
int			exec_builtin(char **argv, t_env *env);
char		*search_path(const char *filename);
void		interpret(char *line, int *stat_loc, t_env *env);
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
void		append_char(char **s, char c);
char		*find_env(char *p);
char		*ft_strjoin(const char *s1, const char *s2);
t_node		*new_node(t_node_kind kind);
t_token		*tokdup(t_token *tok);
void		append_tok(t_token **tokens, t_token *tok);
t_node		*parse(t_token *tok);
t_redirect	*redirect_type(t_token **rest, t_token *tok, char *type);
t_node		*redirect_type2(t_token **rest, t_token *tok, char *type);
void		free_node(t_node *node);
void		free_token(t_token *tok);
void		free_redirects(t_redirect *redir);
char		**ft_split(char const *s, char c);
int			read_heredoc(const char *delimiter, int expand, char *heredoc_file, t_env *env);
char		*expand_variables(char *str, t_env *env);
char		*ft_itoa(int n);
char		*ft_substr(char const *s, unsigned int start, size_t len);
void		open_redir_file(t_redirect *redir);
void		do_redirect(t_redirect *redir);
void		reset_redirect(t_redirect *redir);
void		quote_removal_redirects(t_redirect *redir);
int			setup_heredoc(t_node *node, t_env *env);
int			is_var_char(char c);

#endif
