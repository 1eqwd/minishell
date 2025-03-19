/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:20:29 by sumedai           #+#    #+#             */
/*   Updated: 2025/03/19 14:05:08 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_syntax_err = 0;

void	tokenize_error(const char *location, char **rest, char *line)
{
	g_syntax_err = 1;
	dprintf(STDERR_FILENO, "minishell: syntax error near %s\n", location);
	while (*line)
		line++;
	*rest = line;
}

int is_special_patern(char *line)
{
    while (*line && isdigit(*line))
    {
        // if(*line != '2' || *line != '1')
        //     fatal_error("Bad file descriptor");
        line++;
    }
    if (*line == '>')
        return (1);
    return (0);
}

// int main(void)
// {
//     printf("%d\n",special_patern("1>"));
//     return (0);
// }
t_token *tokenize(char *line)
{
    t_token head;
    t_token *tok;

    head.next = NULL;
    tok = &head;
    while (*line)
    {
        if (consume_blank(&line, line))
            continue;
        else if (is_metacharacter(*line) || is_special_patern(line))
        {
            // printf("in op %s\n",line);
            tok->next = operator(&line, line);
            // printf("%s\n",tok->next->word);
            tok = tok->next;
        }
        else if (is_word(line))
        {
            tok->next = word(&line, line);
            // printf("%s\n",tok->next->word);
            tok = tok->next;
        }
        else
            tokenize_error("Unexpected Token", &line, line);
    }
    tok->next = new_token(NULL, TK_EOF);
    return (head.next);
}

// int main(void)
// {
//     t_token *tok = tokenize("echo asfd 2> test.log");
//     while (tok->kind != TK_EOF)
//     {
//         printf("%s\n",tok->word);
//         tok = tok->next;
//     }
//     return (0);
// }
