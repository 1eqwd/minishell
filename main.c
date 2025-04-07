/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 19:38:22 by mawako            #+#    #+#             */
/*   Updated: 2025/04/06 23:33:19 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	**environ = NULL;
// pid_t	g_last_bg_pid = 0;
// int		g_last_exit_status = 0;

void	fatal_error(const char *msg)
{
	fprintf(stderr, "Fatal error: %s\n", msg);
	exit(1);
}

void	sigint_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	free_strs(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
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

int	wait_pipeline_children(pid_t *pids, int n)
{
	int	i;
	int	status;
	int	sigint_error;

	i = 0;
	status = 0;
	sigint_error = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			sigint_error = 1;
		i++;
	}
	if (sigint_error)
		write(1, "\n", 1);
	return (status);
}

static int	exec_sh_c(char **argv, t_env *env)
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
	setup_heredoc(node);
	status = exec(node, env);
	free_node(node);
	free_token(words);
	return (status);
}

int	exec_pipeline(t_node *head, t_env *env)
{
	int			n;
	int			i;
	int			j;
	int			status;
	int			(*pipes)[2];
	pid_t		*pids;
	t_node		*cur;
	char		**argv;
	char		*cmd_path;

	n = 0;
	pipes = NULL;
	cur = head;
	while (cur)
	{
		n++;
		cur = cur->next;
	}
	if (n > 1)
	{
		pipes = malloc(sizeof(int[2]) * (n - 1));
		if (!pipes)
			fatal_error("malloc failed");
		i = 0;
		while (i < n - 1)
		{
			if (pipe(pipes[i]) == -1)
				fatal_error("pipe failed");
			i++;
		}
	}
	pids = malloc(sizeof(pid_t) * n);
	if (!pids)
		fatal_error("malloc failed");
	cur = head;
	i = 0;
	while (i < n)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			fatal_error("fork failed");
		if (pids[i] == 0)
		{
			if (i > 0)
			{
				if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
					fatal_error("dup2 failed");
			}
			if (i < n - 1)
			{
				if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
					fatal_error("dup2 failed");
			}
			if (pipes)
			{
				j = 0;
				while (j < n - 1)
				{
					close(pipes[j][0]);
					close(pipes[j][1]);
					j++;
				}
			}
			if (cur->redirects)
			{
				open_redir_file(cur->redirects);
				do_redirect(cur->redirects);
			}
			{
				argv = create_argv(cur->args);
				if (!argv || !argv[0])
					exit(0);
				if (is_builtin(argv[0]))
				{
					status = exec_builtin(argv,env);
					free(argv);
					exit(status);
				}
				else
				{
					cmd_path = search_path(argv[0]);
					if (!cmd_path)
						fatal_error("command not found");
					execve(cmd_path, argv, env->environ);
					fatal_error("execve failed");
				}
			}
		}
		cur = cur->next;
		i++;
	}
	if (pipes)
	{
		i = 0;
		while (i < n - 1)
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			i++;
		}
		free(pipes);
	}
	i = 0;
	signal(SIGINT, SIG_IGN);
	status = wait_pipeline_children(pids, n);
	signal(SIGINT, sigint_handler);
	free(pids);
	return (status);
}

int	exec_cmd(t_node *node, t_env *env)
{
	char	**argv;
	char	*path;
	int		status;
	int		wstatus;
	pid_t	pid;

	argv = create_argv(node->args);
	if (!argv || !argv[0])
	{
		free(argv);
		return (0);
	}
	if ((!strcmp(argv[0], "sh") || !strcmp(argv[0], "bash")
			|| !strcmp(argv[0], "zsh"))
		&& argv[1] && !strcmp(argv[1], "-c") && argv[2])
	{
		status = exec_sh_c(argv, env);
		free(argv);
		return (status);
	}
	if (is_builtin(argv[0]) && node->redirects == NULL)
	{
		status = exec_builtin(argv, env);
		free(argv);
		return (status);
	}
	pid = fork();
	if (pid < 0)
		fatal_error("fork failed");
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		open_redir_file(node->redirects);
		do_redirect(node->redirects);
		if (is_builtin(argv[0]))
		{
			status = exec_builtin(argv, env);
			exit(status);
		}
		else
		{
			path = search_path(argv[0]);
			if (!path)
				fatal_error("command not found");
			execve(path, argv, env->environ);
			fatal_error("execve failed");
		}
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &wstatus, 0);
		if (WIFSIGNALED(wstatus) && WTERMSIG(wstatus) == SIGINT)
			write(1, "\n", 1);
		signal(SIGINT, sigint_handler);
		free(argv);
		if (WIFEXITED(wstatus))
			return (WEXITSTATUS(wstatus));
		else if (WIFSIGNALED(wstatus))
			return (128 + WTERMSIG(wstatus));
		return (1);
	}
}

int	exec(t_node *node, t_env *env)
{
	int		status;
	t_node	*pipeline_head;

	status = 0;
	while (node)
	{
		if (node->separator && strcmp(node->separator, "|") == 0)
		{
			pipeline_head = node;
			while (node && node->separator && strcmp(node->separator, "|") == 0)
				node = node->next;
			status = exec_pipeline(pipeline_head, env);
			if (node)
				node = node->next;
		}
		else
		{
			status = exec_cmd(node, env);
			if (node->separator)
			{
				if (!strcmp(node->separator, "&&"))
				{
					if (status != 0)
						break ;
				}
				else if (!strcmp(node->separator, "||"))
				{
					if (status == 0)
						break ;
				}
			}
			node = node->next;
		}
	}
	return (status);
}

void	interpret(char *line, int *stat_loc, t_env *env)
{
	t_token	*words;
	t_node	*node;

	words = tokenize(line);
	if (!words || words->kind == TK_EOF)
	{
		*stat_loc = ERROR_TOKENIZE;
		return ;
	}
	node = parse(words);
	if (!node)
	{
		*stat_loc = 258;
		return ;
	}
	expansion(node, env);
	setup_heredoc(node);
	*stat_loc = exec(node, env);
	free_node(node);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	env;
	int		status;
	char	*line;

	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	(void)argv;
	(void)argc;
	env.g_env = NULL;
	env.environ = NULL;
	env.last_bg_pid = 0;
    env.last_exit_status = 0;
	init_env(envp, &env);
	env.environ = env.g_env;
	status = 0;
	while (1)
	{
		line = readline("minishell$ ");
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
