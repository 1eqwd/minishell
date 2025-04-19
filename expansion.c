/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:05:27 by mawako            #+#    #+#             */
/*   Updated: 2025/04/19 17:25:05 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// extern int	g_exit_status;
// extern int	g_last_exit_status;

void	append_char(char **s, char c)
{
	char	*new;
	size_t	size;

	size = 2;
	if (*s)
		size += strlen(*s);
	new = malloc(size);
	if (!new)
		fatal_error("append_char: malloc");
	if (*s)
	{
		strlcpy(new, *s, size);
		free(*s);
	}
	new[size - 2] = c;
	new[size - 1] = '\0';
	*s = new;
}

char	*find_env(char *p)
{
	char	*env;
	char	*content;
	size_t	size;

	p++;
	size = 0;
	while (p[size] && (p[size] != DUOBLE_QUOTE_CHAR)
		&& (p[size] != SINGLE_QUOTE_CHAR) && !is_blank(p[size]))
		size++;
	env = strndup(p, size);
	if (!env)
		fatal_error("find_env: strndup");
	content = getenv(env);
	free(env);
	if (!content)
		return (NULL);
	return (content);
}

void	remove_quote(t_token *tok, t_env *env)
{
	char	*new_word;
	char	*p;
	int		i;

	if (!tok || tok->kind != TK_WORD || !tok->word)
		return ;
	new_word = strdup("");
	p = tok->word;
	i = 0;
	while (p[i])
	{
		if (p[i] == SINGLE_QUOTE_CHAR)
		{
			i++;
			while (p[i] && p[i] != SINGLE_QUOTE_CHAR)
				append_char(&new_word, p[i++]);
			if (p[i] == SINGLE_QUOTE_CHAR)
				i++;
		}
		else if (p[i] == DUOBLE_QUOTE_CHAR)
		{
			i++;
			while (p[i] && p[i] != DUOBLE_QUOTE_CHAR)
			{
				if (p[i] == '\\' && (p[i + 1] == '\\' || p[i + 1]
						== '"' || p[i + 1] == '$' || p[i + 1] == '`'))
				{
					append_char(&new_word, p[i + 1]);
					i += 2;
				}
				else if (p[i] == '$')
					handle_dollar(p, &i, &new_word, env);
				else
					append_char(&new_word, p[i++]);
			}
			if (p[i] == DUOBLE_QUOTE_CHAR)
				i++;
		}
		else if (p[i] == '\\')
		{
			if (p[i + 1])
			{
				append_char(&new_word, p[i + 1]);
				i += 2;
			}
			else
				i++;
		}
		else if (p[i] == '$')
			handle_dollar(p, &i, &new_word, env);
		else
			append_char(&new_word, p[i++]);
	}
	free(tok->word);
	tok->word = new_word;
	remove_quote(tok->next, env);
}

char	*remove_quote_word(char *word)
{
	char	*new;
	int		i;
	int		j;
	char	quote;

	if (!word)
		return (NULL);
	new = malloc(strlen(word) + 1);
	if (!new)
		fatal_error("remove_quote_word: malloc");
	i = 0;
	j = 0;
	while (word[i])
	{
		if (word[i] == '\'' || word[i] == '\"')
		{
			quote = word[i++];
			while (word[i] && word[i] != quote)
				new[j++] = word[i++];
			if (word[i] == quote)
				i++;
		}
		else
			new[j++] = word[i++];
	}
	new[j] = '\0';
	free(word);
	return (new);
}

void	quote_removal(t_node *node, t_env *env)
{
	if (!node)
		return ;
	if (node->kind == ND_SUBSHELL && node->child)
		quote_removal(node->child, env);
	else
	{
		if (node->args)
			remove_quote(node->args, env);
		if (node->filename)
			remove_quote(node->filename, env);
		quote_removal_redirects(node->redirects);
	}
	if (node->next)
		quote_removal(node->next, env);
}

void	quote_removal_redirects(t_redirect *redir)
{
	char	*new_word;

	while (redir)
	{
		if (redir->word)
		{
			new_word = remove_quote_word(redir->word);
			redir->word = new_word;
		}
		redir = redir->next;
	}
}

void	expansion(t_node *node, t_env *env)
{
	quote_removal(node, env);
}
