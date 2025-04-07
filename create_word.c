/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:04:39 by mawako            #+#    #+#             */
/*   Updated: 2025/03/31 14:43:34 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *word, t_token_kind kind)
{
	t_token	*tok;

	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		fatal_error("new_token: malloc");
	tok->word = word;
	tok->kind = kind;
	tok->next = NULL;
	return (tok);
}

t_token	*operator_func(char **rest, char *line)
{
	size_t				i;
	char				*op;
	static char *const	operators[] = {
		"1>&1", "1>&2", ">&1", "2>&2", ">&2", "2>&1", "2>", "2>>", "1>", "1>>",
		"&>", "&>>", ">>", "<<", ">", "<", "||", "&&", "&", ";", ";;", "(",
		")", "|", "\n", NULL
	};

	i = 0;
	while (operators[i])
	{
		if (startswith(line, operators[i]))
		{
			op = strdup(operators[i]);
			if (!op)
				fatal_error("operator_func: strdup");
			*rest = line + strlen(op);
			return (new_token(op, TK_OP));
		}
		i++;
	}
	fatal_error("Unexpected operator");
	return (NULL);
}

t_token	*word_func(char **rest, char *line)
{
	int		in_squote;
	int		in_dquote;
	char	*start;
	char	*tmp;

	in_squote = 0;
	in_dquote = 0;
	start = line;
	while (*line)
	{
		if (*line == SINGLE_QUOTE_CHAR && !in_dquote)
			in_squote = !in_squote;
		else if (*line == DUOBLE_QUOTE_CHAR && !in_squote)
			in_dquote = !in_dquote;
		if (!in_squote && !in_dquote)
		{
			if (is_blank(*line) || is_metacharacter(*line)
				|| check_redirect(line))
				break ;
		}
		line++;
	}
	tmp = strndup(start, line - start);
	if (!tmp)
		fatal_error("word_func: strndup");
	*rest = line;
	return (new_token(tmp, TK_WORD));
}
