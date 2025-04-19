/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:42:10 by mawako            #+#    #+#             */
/*   Updated: 2025/04/19 17:49:04 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_exec_node(t_node **node_ptr, int *exit_loop, t_env *env)
{
	t_node	*node;
	t_node	*tail;
	int		status;
	char	*sep;
	char	*logical_sep;

	node = *node_ptr;
	status = 0;
	*exit_loop = 0;
	if (node->kind == ND_SUBSHELL)
	{
		sep = node->separator;
		status = handle_subshell(node_ptr, env);
		if (sep)
		{
			if (!strcmp(sep, "&&") && status != 0)
				*exit_loop = 1;
			else if (!strcmp(sep, "||") && status == 0)
				*exit_loop = 1;
		}
	}
	else if (node->separator && !strcmp(node->separator, "|"))
	{
		tail = node;
		while (tail->separator && !strcmp(tail->separator, "|"))
			tail = tail->next;
		logical_sep = tail->separator;

		status = handle_pl(node_ptr, env);
		if (logical_sep)
		{
			if (!strcmp(logical_sep, "&&") && status != 0)
				*exit_loop = 1;
			else if (!strcmp(logical_sep, "||") && status == 0)
				*exit_loop = 1;
		}
	}
	else if (node->separator && !strcmp(node->separator, "&"))
		status = handle_bg(node_ptr, env);
	else
	{
		status = exec_cmd(node, env);
		if (handle_logical_separator(node_ptr, status))
			*exit_loop = 1;
	}

	return (status);
}

int	exec_tree(t_node *node, t_env *env)
{
	int	status;
	int	exit_loop;

	status = 0;
	exit_loop = 0;
	while (node)
	{
		status = process_exec_node(&node, &exit_loop, env);
		if (exit_loop)
			break ;
	}
	return (status);
}
