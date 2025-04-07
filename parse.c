/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:04:59 by mawako            #+#    #+#             */
/*   Updated: 2025/04/03 15:23:19 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*new_node(t_node_kind kind)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		fatal_error("new_node: malloc");
	node->kind = kind;
	node->args = NULL;
	node->filename = NULL;
	node->next = NULL;
	node->redirects = NULL;
	node->separator = NULL;
	node->targetfd = 0;
	node->targetfd2 = 0;
	node->filefd = 0;
	node->stashed_targetfd = 0;
	node->stashed_targetfd2 = 0;
	return (node);
}

t_token	*tokdup(t_token *tok)
{
	char	*word;

	if (!tok)
		return (NULL);
	word = strdup(tok->word);
	if (!word)
		fatal_error("tokdup: strdup");
	return (new_token(word, tok->kind));
}

void	append_redirect(t_redirect **list, t_redirect *new_redir)
{
	t_redirect	*cur;

	if (!list || !new_redir)
		return ;
	if (*list == NULL)
		*list = new_redir;
	else
	{
		cur = *list;
		while (cur->next)
			cur = cur->next;
		cur->next = new_redir;
	}
}

static void	append_node(t_node **nodes, t_node *node)
{
	if (!*nodes)
	{
		*nodes = node;
		return ;
	}
	append_node(&(*nodes)->next, node);
}

void	append_tok(t_token **tokens, t_token *tok)
{
	if (!*tokens)
	{
		*tokens = tok;
		return ;
	}
	append_tok(&(*tokens)->next, tok);
}

static int	is_redirect(const char *word)
{
	int			i;
	static char *const	redirs[] = {
		">>", "2>>", "<<", "<", ">", "2>", "1>", ">&1", "1>&1",
		"1>&2", ">&2", "2>&2", "2>&1", "&>", ">&", NULL
	};

	i = 0;
	if (!word)
		return (0);
	while (redirs[i])
	{
		if (!strcmp(redirs[i], word))
			return (1);
		i++;
	}
	return (0);
}

static int	is_redirect_op(t_token *tok)
{
	if (!tok || tok->kind != TK_OP)
		return (0);
	return (is_redirect(tok->word));
}

static int	is_control_operator(t_token *tok)
{
	if (!tok || tok->kind != TK_OP)
		return (0);
	if (!strcmp(tok->word, ";"))
		return (1);
	if (!strcmp(tok->word, "&&"))
		return (1);
	if (!strcmp(tok->word, "||"))
		return (1);
	if (!strcmp(tok->word, "|"))
		return (1);
	if (!strcmp(tok->word, "&"))
		return (1);
	return (0);
}

static t_node	*parse_simple_command(t_token **rest)
{
	t_node	*node;
	t_token	*tok;

	tok = *rest;
	node = new_node(ND_SIMPLE_CMD);
	while (tok && tok->kind != TK_EOF && !is_control_operator(tok))
	{
		if (tok->kind == TK_WORD)
		{
			append_tok(&node->args, tokdup(tok));
			tok = tok->next;
		}
		else if (is_redirect_op(tok))
		{
			if (!tok->next || tok->next->kind != TK_WORD)
			{
				fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
				return (NULL);
			}
			append_redirect(&node->redirects, redirect_type(&tok, tok, tok->word));
		}
		else
			fatal_error("parse_simple_command: unexpected token");
	}
	*rest = tok;
	return (node);
}

t_node	*parse(t_token *tok)
{
	t_node	*head;
	t_node	*cur;
	t_node	*next_cmd;

	head = NULL;
	cur = NULL;
	while (tok && tok->kind != TK_EOF)
	{
		next_cmd = parse_simple_command(&tok);
		if (!next_cmd)
			return (NULL);
		if (!head)
			head = next_cmd;
		else
			append_node(&cur->next, next_cmd);
		cur = next_cmd;
		if (is_control_operator(tok))
		{
			cur->separator = strdup(tok->word);
			tok = tok->next;
		}
	}
	return (head);
}
