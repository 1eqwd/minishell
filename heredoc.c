/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:05:29 by mawako            #+#    #+#             */
/*   Updated: 2025/04/19 17:38:41 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_and_write(int fd, char *line, int expand, t_env *env)
{
	char	*expanded;

	if (expand)
		expanded = expand_variables(line, env);
	else
		expanded = strdup(line);
	if (expanded)
	{
		write(fd, expanded, strlen(expanded));
		write(fd, "\n", 1);
		free(expanded);
	}
}

int	read_heredoc(const char *delimiter, int expand, char *heredoc_file, t_env *env)
{
	char	*line;
	int		fd;

	fd = open(heredoc_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		expand_and_write(fd, line, expand, env);
		free(line);
	}
	close(fd);
	return (0);
}

int	setup_heredoc(t_node *node, t_env *env)
{
	t_redirect	*redir;
	static int	heredoc_count;
	char		filename[256];

	redir = node->redirects;
	heredoc_count = 0;
	while (redir)
	{
		if (redir->type == RD_HEREDOC)
		{
			snprintf(filename, sizeof(filename),
				"/tmp/.heredoc_%d", heredoc_count++);
			redir->heredoc_filename = strdup(filename);
			redir->heredoc_expand = !redir->quoted;
			read_heredoc(redir->word, redir->heredoc_expand, filename, env);
		}
		redir = redir->next;
	}
	return (0);
}
