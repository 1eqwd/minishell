/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 16:17:26 by sumedai           #+#    #+#             */
/*   Updated: 2025/03/18 19:16:13 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_blank(char c)
{
    return ((c == ' ') || (c == '\t') || (c == '\n'));
}

int consume_blank(char **rest, char *line)
{
    if (is_blank(*line))
    {
        while (*line && is_blank(*line))
            line++;
        *rest = line;
        return (1);
    }
    *rest = line;
    return (0);
}

int startswith(const char *s, const char *keyword)
{
    return ((memcmp(s, keyword, strlen(keyword)) == 0));
}

// int is_operator(const char *s)
// {
//     static char *const operators[] = {">>","<<","<","" ,">&2",,,,,,,"2>&1","2>",">","||" , "&", "&&", ";", ";;", "(", ")", "|", "\n", NULL};
//     size_t i = 0;
    
//     while (operators[i])
//     {
//         if (startswith(s, operators[i++]))
//             return (1);
//     }
//     return (0);
// }

int check_redirect(const char *s)
{
    int i = 0;
    if (((s[i] == '1' || s[i] == '2')) && (s[i + 1] == '<' || s[i + 1] == '>'))
        return (1);
    return (0);
}

int is_metacharacter(char c)
{
    return (c && strchr("|&;()<> \t\n", c));
}

int is_word(const char *s)
{
    return (*s && !is_metacharacter(*s));
}