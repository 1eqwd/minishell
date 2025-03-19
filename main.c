/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:52:26 by sumedai           #+#    #+#             */
/*   Updated: 2025/03/19 14:04:02 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void interpret(char *line, int *stat_loc)
{
	t_token *words;
    t_node *node;

	words = tokenize(line);                               
	if (words->kind == TK_EOF)
	{
		// printf("into if\n");
		*stat_loc = ERROR_TOKENIZE;
	}
    else
    {
		node = parse(words);
		expansion(node);
        *stat_loc = exec(node);
    	// free_node(node);
	}
}

char *search_path(const char *filename)
{
	char path[PATH_MAX];
	char *value;
	char *end;

	value = getenv("PATH");
	// printf("ENV:%s\n",value);
	while (*value)
	{
		bzero(path, PATH_MAX);
		end = strchr(value, ':');
		if (end)
			strncpy(path, value, end - value);
		else
			strcpy(path, value);
		strlcat(path, "/", PATH_MAX);
		strlcat(path, filename, PATH_MAX);
		// printf("path:%s\n", path);
		if ((access(path, X_OK) == 0) && (access(path, F_OK) == 0))
		{
			char *dup;

			dup = strdup((const char *)path);
			if (!dup)
				fatal_error("strdup");
			return (dup);
		}
		value = end + 1;
	}
	return (NULL);
}

int exec_cmd(t_node *node)
{
	extern char **environ;
	char	**argv;
    int		status;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		fatal_error("fork");
	else if (pid == 0)
	{
		argv = create_argv(node->args);
		if (strchr(argv[0] ,'/') == NULL)
			argv[0] = search_path(argv[0]);
		else if (access(argv[0], X_OK) < 0 && access(argv[0] , F_OK) < 0)
			fatal_error("execve");
		// printf("command:%s argument:%s\n",argv[0], argv[1]);
		execve(argv[0], argv, environ);
	}
	else
	{
		wait(&status);
		return (WIFEXITED(status));
	}
    return (0);
}

int  exec(t_node *node)
{
	int status;

	// printf("in exec\n");
	open_redir_file(node->redirects);
	do_redirect(node->redirects);
	status = exec_cmd(node);
	// printf("%d\n", status);
	reset_redirect(node->redirects);
    return (status);
}

int	main(void)
{
	int		status;
	char	*line;

	rl_outstream = stderr;
	status = 0;
	while (42)
	{
		line = readline("minishell$");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		interpret(line, &status);
		free(line);
	}
	exit(status);
}
