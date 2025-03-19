/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_argv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 16:12:53 by sumedai           #+#    #+#             */
/*   Updated: 2025/03/16 17:38:36 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **allocate_argv(t_token *tok, size_t size)
{
	char **heap;

	// printf("size=%zu\n", size);
	heap = (char **)malloc(sizeof(char *) * (size + 1));
	if (!heap)
		return(NULL);
	size_t i = 0;
	while (tok)
	{
		heap[i] = (char *)malloc(sizeof(char) * (strlen(tok->word)));
		if (!heap[i])
		{
			//argv[i]をfreeする処理を書く
		}
		i++;
		tok = tok->next;
	}
	// printf("%zu\n", i);
	return (heap);
}

char **create_argv(t_token *tok)
{
	// printf("create_argv in\n");
	char **argv;
	size_t size = 0;
	t_token *tmp;

	tmp = tok;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	argv = allocate_argv(tok, size);
	if (!argv)
		return (NULL);
	size_t i = 0;
	while ((i < size) && tok)
	{
		argv[i] = tok->word;
		i++;
		tok = tok->next;
	}
	argv[i] = NULL;
	// printf("%s\n", argv[0]);
	return (argv);
}