/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 19:38:22 by mawako            #+#    #+#             */
/*   Updated: 2025/04/19 17:42:22 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env_management.h"

// char	**environ = NULL;
// pid_t	g_last_bg_pid = 0;
// int		g_last_exit_status = 0;

void	fatal_error(const char *msg)
{
	fprintf(stderr, "Fatal error: %s\n", msg);
	exit(1);
}

char	*search_path(const char *filename)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	int		i;

	if (!filename || strchr(filename, '/'))
		return (strdup(filename));
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = malloc(strlen(paths[i]) + strlen(filename) + 2);
		if (!full_path)
			break ;
		sprintf(full_path, "%s/%s", paths[i], filename);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	free_strs(paths);
	return (NULL);
}

int	exec_sh_c(char **argv, t_env *env)
{
	char	*subcmd;
	int		status;
	t_token	*words;
	t_node	*node;

	subcmd = argv[2];
	status = 0;
	words = tokenize(subcmd);
	if (!words || words->kind == TK_EOF)
		return (0);
	node = parse(words);
	if (!node)
	{
		free_token(words);
		return (258);
	}
	expansion(node, env);
	setup_heredoc(node, env);
	status = exec(node , env);
	free_node(node);
	free_token(words);
	return (status);
}

int	check_quote(const char *line)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !dq)
			sq = !sq;
		else if (line[i] == '"' && !sq)
			dq = !dq;
		i++;
	}
	return (!sq && !dq);
}

static int	ends_with_unescaped_bs(const char *line)
{
	int	len;
	int	count;

	len = strlen(line);
	count = 0;
	while (len > 0 && line[--len] == '\\')
		count++;
	if (count % 2 == 1)
		return (1);
	return (0);
}

char	*get_complete_input(void)
{
	char	*line;
	char	*more;
	char	*new;

	line = readline("minishell$ ");
	if (!line)
		return (NULL);
	while (!check_quote(line) || ends_with_unescaped_bs(line))
	{
		more = readline("> ");
		if (!more)
			break ;
		if (ends_with_unescaped_bs(line))
			line[strlen(line) - 1] = '\0';
		new = ft_strjoin(line, more);
		free(line);
		line = new;
		free(more);
	}
	return (line);
}

int	main(int argc, char **argv, char **envp)
{
	int			status;
	char		*line;
	t_env 		env;
	struct sigaction	sa;

	(void)argv;
	(void)argc;
	env.g_env = NULL;
	env.environ = NULL;
	env.last_bg_pid = 0;
    env.last_exit_status = 0;
	init_env(envp, &env);
	env.environ = env.g_env;
	status = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGCHLD, &sa, NULL);
	while (1)
	{
		line = get_complete_input();
		if (!line)
			break ;
		if (*line)
			add_history(line);
		interpret(line, &status, &env);
		free(line);
	}
	free_env(&env);
	return (status);
}
