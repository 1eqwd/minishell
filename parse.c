/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:14:42 by sumedai           #+#    #+#             */
/*   Updated: 2025/03/19 14:08:18 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node *new_node(t_node_kind kind)
{
    t_node *node;
    
    node = malloc(sizeof(t_node));
    if (!node)
        fatal_error("calloc");
    node->kind = kind;
    node->args = NULL;
    node->filename = NULL;
    node->next = NULL;
    return (node);
}

t_token *tokdup(t_token *tok)
{
    char *word;

    if (!tok)
        return (NULL);
    word = strdup(tok->word);
    if (!word)
        fatal_error("strdup");
    return (new_token(word, tok->kind));
}

void append_tok(t_token **tokens, t_token *tok)
{
    if (*tokens == NULL)
    {
        *tokens = tok;
        return ;
    }
    append_tok(&(*tokens)->next, tok);
}

void append_node(t_node **nodes, t_node *node)
{
    if (*nodes == NULL)
    {
        *nodes = node;
        return ;
    }
    append_node(&(*nodes)->next, node);
}

int is_redirect(char *word)
{
    // printf("in is_redirect %s\n",word);
    static char *const redirect[] = {">>","2>>","<<", "<", ">", "2>","1>",">&1","1>&1","1>&2",">&2","2>&2","2>&1","&>",">&",NULL};
    int i = 0;
    while(redirect[i])
    {
       if(strcmp(redirect[i], word) == 0)
       {
            // printf("%d\n",1);
            return (1);
       }
        i++;
    }
    return (0);
}

t_node *parse(t_token *tok)
{
    t_node *node;

    // t_token *tmp;
    // tmp = tok;
    // printf("in parse\n");
    // while (tmp)
    // {
    //     printf("%s\n",tmp->word);
    //     tmp = tmp->next;
    // }
    node = new_node(ND_SIMPLE_CMD);
    while (tok && (tok->kind != TK_EOF))
    {
        // printf("before if, else tok %s kind:%u\n",tok->word, tok->kind);
        if (tok->kind == TK_WORD)
        {
            // printf("parse into append_tok %s\n",tok->word);
            append_tok(&node->args, tokdup(tok));
            tok = tok->next;
        }
        else if ((is_redirect(tok->word)) && (tok->kind == TK_OP) && (tok->next->kind == TK_WORD))
        {
            if(strcmp(tok->word, ">") == 0)
                append_node(&node->redirects, redirect_type(&tok, tok, ">"));
            else if (strcmp(tok->word, "2>") == 0)
                append_node(&node->redirects, redirect_type(&tok, tok, "2>"));
            else if (strcmp(tok->word, "1>") == 0)
                append_node(&node->redirects, redirect_type(&tok, tok, "1>"));
            else if (strcmp(tok->word ,"<") == 0)
                append_node(&node->redirects, redirect_type(&tok, tok, "<"));
            else if (strcmp(tok->word, ">>") == 0)
                append_node(&node->redirects, redirect_type(&tok, tok, ">>"));
            else if (strcmp(tok->word, "1>>") == 0)
                append_node(&node->redirects, redirect_type(&tok, tok, "1>>"));
            else if (strcmp(tok->word, "2>>") == 0)
                append_node(&node->redirects, redirect_type(&tok, tok, "2>>"));
            else if (strcmp(tok->word, "<<") == 0)
                append_node(&node->redirects, redirect_type(&tok, tok, "<<"));
            else if (strcmp(tok->word, "&>") == 0)
                 append_node(&node->redirects, redirect_type(&tok, tok, "&>"));
            // else if (strcmp(tok->word, "&>>") == 0)
            //     append_node(&node->redirects, redirect_type(&tok, tok, "&>>"));
        }
        else if ((is_redirect(tok->word)) && (tok->kind == TK_OP))
        {
            if (strcmp(tok->word, "2>&1") == 0)
                append_node(&node->redirects, redirect_type2(&tok, tok, "2>&1"));
            else if (strcmp(tok->word, "1>&1") == 0)
                 append_node(&node->redirects, redirect_type2(&tok, tok, "1>&1"));
            else if (strcmp(tok->word, "1>&2") == 0)
                append_node(&node->redirects, redirect_type2(&tok, tok, "1>&2"));
            else if (strcmp(tok->word, ">&1") == 0)
                append_node(&node->redirects, redirect_type2(&tok, tok, ">&1"));
            else if (strcmp(tok->word, "2>&2") == 0)
                append_node(&node->redirects, redirect_type2(&tok, tok, "2>&2"));
            else if (strcmp(tok->word, ">&2") == 0)
                append_node(&node->redirects, redirect_type2(&tok, tok, ">&2"));
        }
        else
            fatal_error(" unexpected token `newline'");
    }
    // t_token *tmp = node->args;
    // while (tmp)
    // {
    //     printf("%s\n",tmp->word);
    //     tmp = tmp->next;
    // }
    // printf("%s\n",node->redirects->filename->word);
    return (node);
}
