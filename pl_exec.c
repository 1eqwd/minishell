/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:04:53 by mawako            #+#    #+#             */
/*   Updated: 2025/04/19 17:36:12 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_node *node, t_env *env)
{
	char	**argv;
	char	*cmd_path;

	argv = create_argv(node->args);
	if (!argv || !argv[0])
		exit(0);
	if (is_builtin(argv[0]))
		exit(exec_builtin(argv, env));
	else
	{
		cmd_path = search_path(argv[0]);
		if (!cmd_path)
			fatal_error("command not found");
		execve(cmd_path, argv, env->environ);
		fatal_error("execve failed");
	}
}

void	setup_pipe_child(t_node *node, int i, int n, int **pipes, t_env *env)
{
	setup_dup(i, n, pipes);
	close_all_pipes(n, pipes);
	if (node->redirects)
	{
		if (open_redir_file(node->redirects) < 0)
			exit(1);
		do_redirect(node->redirects);
	}
	execute_command(node, env);
}

pid_t	*setup_pipe_children(t_node *head, int n, int **pipes, t_env *env)
{
	pid_t	*pids;
	t_node	*cur;
	int		i;

	i = 0;
	pids = malloc(sizeof(pid_t) * n);
	if (!pids)
		fatal_error("malloc failed");
	cur = head;
	while (i < n)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			fatal_error("fork failed");
		if (pids[i] == 0)
			setup_pipe_child(cur, i, n, pipes, env);
		cur = cur->next;
		i++;
	}
	return (pids);
}

int	wait_pl_children(pid_t *pids, int n)
{
	int	i;
	int	status;
	int	sigint_error;

	i = 0;
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
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	exec_pl(t_node *head, t_env *env)
{
	int		n;
	int		status;
	pid_t	*pids;
	int		**pipes;

	n = count_pl_nodes(head);
	pipes = setup_pipes(n);
	pids = setup_pipe_children(head, n, pipes, env);
	cleanup_pipes(pipes, n);
	signal(SIGINT, SIG_IGN);
	status = wait_pl_children(pids, n);
	signal(SIGINT, sigint_handler);
	free(pids);
	return (status);
}
