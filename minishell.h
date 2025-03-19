/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:15:01 by sumedai           #+#    #+#             */
/*   Updated: 2025/03/18 22:47:57 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#define SINGLE_QUOTE_CHAR '\''
#define DUOBLE_QUOTE_CHAR '\"'
#define ERROR_TOKENIZE 3
#define STDOUT 1
#define STDIN 0
#define STDERR 2
#include <limits.h> 
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef enum e_token_kind
{
	TK_WORD,
	TK_RESERVED,
	TK_OP,
	TK_EOF,
}   t_token_kind;

typedef struct s_token 
{
	char			*word;
	t_token_kind	kind;
	struct s_token	*next;

}   t_token;

typedef enum e_node_kind
{
	ND_SIMPLE_CMD,
	ND_REDIR_OUT,
	ND_REDIR_IN,
	ND_REDIR_OUT_ERR,
	ND_REDIR_SP,
	ND_REDIR_APPEND_OUT,
	ND_REDIR_APPEND_IN,
} t_node_kind;

typedef struct s_node
{
	t_token		*args;
	t_node_kind	kind;
 	struct s_node *next;
 	struct s_node		*redirects;
 	int			targetfd;
	int			targetfd2;
 	t_token		*filename;
 	int			filefd;
 	int			stashed_targetfd;
	int			stashed_targetfd2;
} t_node;

//main.c
void	fatal_error(const char *msg) __attribute__((noreturn));
char *search_path(const char *filename);
void interpret(char *line,  int *stat_loc);
char **allocate_argv(t_token *tok, size_t size);
char **create_argv(t_token *tok);
int exec_cmd(t_node *node);
int  exec(t_node *node);


//tokenizer.c
t_token *new_token(char *word, t_token_kind kind);
int is_blank(char c);
int consume_blank(char **rest, char *line);
int startswith(const char *s, const char *keyword);
int is_operator(const char *s);
int is_metacharacter(char c);
int is_word(const char *s);
int is_special_patern(char *line);
t_token *operator(char **rest, char *line);
t_token *word(char **rest, char *line);
t_token *tokenize(char *line);
void	tokenize_error(const char *location, char **rest, char *line);
int check_redirect(const char *s);

//expansion.c
void expansion(t_node *node);
void quote_removal(t_node *node);
void remove_quote(t_token *tok);
void append_char(char **s, char c);
char *find_env(char *p);
char	*ft_strjoin(char const *s1, char const *s2);

//parse.c
t_node *new_node(t_node_kind kind);
t_token *tokdup(t_token *tok);
void append_tok(t_token **tokens, t_token *tok);
t_node *parse(t_token *tok);


//redirect.c
void open_redir_file(t_node *redir);
void do_redirect(t_node *redir);
void reset_redirect(t_node *redir);

//redirect_type.c
t_node *redirect_type2(t_token **rest, t_token *tok, char *type);
t_node *redirect_type(t_token **rest, t_token *tok, char *type);

#endif
