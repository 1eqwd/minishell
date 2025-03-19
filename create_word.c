/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 16:15:05 by sumedai           #+#    #+#             */
/*   Updated: 2025/03/19 14:11:11 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void fatal_error(const char *msg)
{
	printf("Fatal Error: %s\n", msg);
	exit(1);
}

t_token *new_token(char *word, t_token_kind kind)
{
    t_token *tok;
    
    tok = (t_token *)malloc(sizeof(t_token));
    if (!tok)
        fatal_error("malloc");
    tok->word = word;
    tok->kind = kind;
    tok->next = NULL;
    return (tok);
}

t_token *operator(char **rest, char *line)
{
    static char *const operators[] = {"1>&1","1>&2",">&1","2>&2",">&2","2>&1", "2>", "2>>","1>","1>>","&>","&>>",">>","<<", ">","<","||", "&", "&&", ";", ";;", "(", ")", "|", "\n", NULL};
    size_t i = 0;
    char *op;
    
    while (operators[i])
    {
        if (startswith(line, operators[i]))
        {
            op = strdup(operators[i]);
            printf("in operator %s\n",op);
            if (!op)
                fatal_error("strdup");
            *rest = line + strlen(op);
            return (new_token(op, TK_OP));
        }
        i++;
    }
    fatal_error("Unexpected operator");
}

t_token *word(char **rest, char *line)
{
    const char *start = line;
    char    *word;

    while (*line && !is_metacharacter(*line) && !check_redirect(line))
    {
        if (*line == SINGLE_QUOTE_CHAR)
        {
            line++;
            while (*line != SINGLE_QUOTE_CHAR)
                line++;
            line++;
        }
        else if (*line == DUOBLE_QUOTE_CHAR)
        {
            line++;
            while (*line != DUOBLE_QUOTE_CHAR)
                line++;
            line++;
        }
        else
            line++;
    }
    word = strndup(start, line - start);
    if (!word)
        fatal_error("strndup");
    *rest = line;
    return (new_token(word, TK_WORD));
}
