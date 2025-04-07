/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:05:17 by mawako            #+#    #+#             */
/*   Updated: 2025/03/27 19:08:22 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_blank(char c)
{
	return ((c == ' ') || (c == '\t') || (c == '\n'));
}

int	consume_blank(char **rest, char *line)
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

int	startswith(const char *s, const char *keyword)
{
	if (!s || !keyword)
		return (0);
	return (strncmp(s, keyword, strlen(keyword)) == 0);
}

int	check_redirect(const char *s)
{
	if (!s || !*s)
		return (0);
	if (isdigit((unsigned char)s[0]) && (s[1] == '<' || s[1] == '>'))
		return (1);
	return (0);
}

int	is_metacharacter(char c)
{
	return (c && strchr("|&;()<> \t\n", c));
}

int	is_word(const char *s)
{
	return (*s && !is_metacharacter(*s));
}

void	tokenize_error(const char *location, char **rest, char *line)
{
	dprintf(STDERR_FILENO, "minishell: syntax error near %s\n", location);
	while (*line)
		line++;
	*rest = line;
}
