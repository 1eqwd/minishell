/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:20:46 by mawako            #+#    #+#             */
/*   Updated: 2025/04/19 17:48:20 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	interpret(char *line, int *status, t_env *env)
{
	t_token	*words;
	t_node	*node;

	words = tokenize(line);
	if (!words || words->kind == TK_EOF)
	{
		*status = ERROR_TOKENIZE;
		free_token(words);
		return ;
	}
	node = parse(words);
	free_token(words);
	if (!node)
	{
		*status = 258;
		return ;
	}
	expansion(node, env);
	setup_heredoc(node, env);
	*status = exec_tree(node, env);
	free_node(node);
}
