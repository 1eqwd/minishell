/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:09:33 by shuu              #+#    #+#             */
/*   Updated: 2025/03/19 14:03:49 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node *redirect_type(t_token **rest, t_token *tok, char *type)
{
    t_node *node;

    if (!tok->next)
        fatal_error(" unexpected token `newline'");
    // printf("redirect_type %s\n",tok->word);
    if (strcmp(type, ">") == 0 || strcmp(type, "1>") == 0)
    {
        node = new_node(ND_REDIR_OUT);
        node->targetfd = STDOUT;
    }
    else if(strcmp(type, "<") == 0)
    {
        node = new_node(ND_REDIR_IN);
        node->targetfd = STDIN;
    }
    else if(strcmp(type, ">>")== 0||strcmp(type, "1>>") == 0)
    {
        node = new_node(ND_REDIR_APPEND_OUT);
        node->targetfd = STDOUT;
    }
    else if (strcmp(type, "2>") == 0)
    {
        node = new_node(ND_REDIR_OUT);
        node->targetfd = STDERR;
    }
    else if (strcmp(type, "2>>") == 0)
    {
        node = new_node(ND_REDIR_APPEND_OUT);
        node->targetfd = STDERR;
    }
    else if (strcmp(type, "&>") == 0)
    {
        // printf("in type %s\n",type);
        node = new_node(ND_REDIR_OUT_ERR);
        node->targetfd = STDOUT;
        node->targetfd2 = STDERR;
    }
    // else if (strcmp(type, "&>>") == 0)
    else
    {
        node = new_node(ND_REDIR_APPEND_IN);
        node->targetfd = STDIN;
    }

    node->filename = tokdup(tok->next);
    *rest = tok->next->next;
    return (node);
}

t_node *redirect_type2(t_token **rest, t_token *tok, char *type)
{
    t_node* node;

    // printf("in redirect_type2 type=%s\n",type);
    if (strcmp(type, ">&1") == 0 || strcmp(type, "1>&1") == 0)
    {
        node = new_node(ND_REDIR_SP);
        node->targetfd = STDOUT;
        node->filefd = STDOUT;
    }
    else if(strcmp(type, ">&2") == 0)
    {
        printf("in >&2\n");
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
        return NULL;
    node->filename = NULL;
    *rest = tok->next;
    return (node);
}
