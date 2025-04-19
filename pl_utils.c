/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:04:40 by mawako            #+#    #+#             */
/*   Updated: 2025/04/11 15:14:13 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pl_nodes(t_node *head)
{
	int	n;

	n = 0;
	while (head)
	{
		n++;
		head = head->next;
	}
	return (n);
}

int	**setup_pipes(int n)
{
	int	**pipes;
	int	i;

	if (n < 2)
		return (NULL);
	pipes = malloc(sizeof(int *) * (n - 1));
	if (!pipes)
		fatal_error("malloc failed");
	i = 0;
	while (i < n - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
			fatal_error("malloc failed");
		if (pipe(pipes[i]) == -1)
			fatal_error("pipe failed");
		i++;
	}
	return (pipes);
}

void	cleanup_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	if (!pipes)
		return ;
	while (i < n - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	setup_dup(int i, int n, int **pipes)
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
}

void	close_all_pipes(int n, int **pipes)
{
	int	j;

	j = 0;
	while (j < n - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}
