/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 10:56:42 by sumedai           #+#    #+#             */
/*   Updated: 2025/03/18 02:20:47 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void append_char(char **s, char c)
{
    size_t size;
    char *new;
    size = 2;
    if(*s)
        size += strlen(*s);
    new = malloc(size);
    if (!new)
        fatal_error("malloc");
    if (*s)
        strlcpy(new, *s, size);
    new[size - 2] = c;
    new[size - 1] = '\0';
    if (*s)
        free(*s);
    *s = new;
}

char *find_env(char *p)
{
    char *env;
    char *content;
    size_t size = 0;
    
    p++;
    while (p[size] && (p[size] != DUOBLE_QUOTE_CHAR))
    {
        env = strndup(p, (size + 1));
        content = getenv(env);
        if (content)
        {
            if (p[size + 1] && isalpha(p[size + 1]))
            {
                write(1, "\n", 1);
                free(env);
                return (NULL);
            }
            content = ft_strjoin(content, &p[++size]);
            free(env);
            return (content);
        }
        size++;
    }
    write(1, "\n", 1);
    free(env);
    return (NULL);
}

void remove_quote(t_token *tok)
{
    char *new_word;
    char *p;
    // char *env;
    int i = 0;

    if (!tok || (tok->kind != TK_WORD) || !tok->word)
        return ;
    p = tok->word;
    new_word = NULL;
    while (p[i] && !is_metacharacter(p[i]))
    {
        if (p[i] == SINGLE_QUOTE_CHAR)
        {
            i++;
            while (p[i] != SINGLE_QUOTE_CHAR)
                append_char(&new_word, p[i++]);
            i++;
        }
        else if (p[i] == DUOBLE_QUOTE_CHAR)
        {
            i++;
            while (p[i] != DUOBLE_QUOTE_CHAR)
            {
                if (p[i] == '$')
                {
                    p = find_env(&p[i]);
                    if (!p)
                        break;
                }
                append_char(&new_word, p[i++]);
            }
            i++;
        }
        else
        {
            if (p[i] == '$')
            {
                p = find_env(&p[i]);
                if (!p)
                    break;
            }
            append_char(&new_word, p[i++]);
        }
    }
    tok->word = new_word;
    remove_quote(tok->next);
}

void quote_removal(t_node *node)
{
   if (!node)
    return ;
    // printf("%s\n",node->args->word);
    remove_quote(node->args);
    remove_quote(node->filename);
    quote_removal(node->redirects);
    quote_removal(node->next);
}

void expansion(t_node *node)
{
    quote_removal(node);
}
