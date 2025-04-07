/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:05:06 by mawako            #+#    #+#             */
/*   Updated: 2025/03/28 19:41:09 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirect	*redirect_type(t_token **rest, t_token *tok, char *type)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		fatal_error("redirect_type: malloc");
	redir->next = NULL;
	redir->quoted = 0;
	redir->heredoc_expand = 0;
	redir->heredoc_filename = NULL;
	redir->filefd = 0;
	redir->stashed_targetfd = 0;
	redir->targetfd2 = 0;
	redir->stashed_targetfd2 = 0;
	if (!tok->next)
		fatal_error("redirect_type: unexpected token `newline'");
	if (!strcmp(type, "<<"))
	{
		redir->type = R_HEREDOC;
		redir->targetfd = STDIN;
	}
	else if (!strcmp(type, "<"))
	{
		redir->type = ND_REDIR_IN;
		redir->targetfd = STDIN;
	}
	else if (!strcmp(type, ">"))
	{
		redir->type = ND_REDIR_OUT;
		redir->targetfd = STDOUT;
	}
	else if (!strcmp(type, ">>"))
	{
		redir->type = ND_REDIR_APPEND_OUT;
		redir->targetfd = STDOUT;
	}
	else if (!strcmp(type, "2>"))
	{
		redir->type = ND_REDIR_OUT;
		redir->targetfd = STDERR;
	}
	else if (!strcmp(type, "2>>"))
	{
		redir->type = ND_REDIR_APPEND_OUT;
		redir->targetfd = STDERR;
	}
	redir->word = strdup(tok->next->word);
	*rest = tok->next->next;
	return (redir);
}

t_node	*redirect_type2(t_token **rest, t_token *tok, char *type)
{
	t_node	*node;

	if (strcmp(type, ">&1") == 0 || strcmp(type, "1>&1") == 0)
	{
		node = new_node(ND_REDIR_SP);
		node->targetfd = STDOUT;
		node->filefd = STDOUT;
	}
	else if (strcmp(type, ">&2") == 0)
	{
		node = new_node(ND_REDIR_SP);
		node->targetfd = STDOUT;
		node->filefd = STDERR;
	}
	else if (strcmp(type, "2>&1") == 0)
	{
		node = new_node(ND_REDIR_SP);
		node->targetfd = STDERR;
		node->filefd = STDOUT;
	}
	else if (strcmp(type, "1>&2") == 0)
	{
		node = new_node(ND_REDIR_SP);
		node->targetfd = STDOUT;
		node->filefd = STDERR;
	}
	else if (strcmp(type, "2>&2") == 0)
	{
		node = new_node(ND_REDIR_SP);
		node->targetfd = STDERR;
		node->filefd = STDERR;
	}
	else
		return (NULL);
	node->filename = NULL;
	*rest = tok->next;
	return (node);
}
