/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_argv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:05:23 by mawako            #+#    #+#             */
/*   Updated: 2025/03/31 17:54:30 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**allocate_argv(t_token *tok, size_t size)
{
	char	**heap;
	size_t	i;

	heap = (char **)malloc(sizeof(char *) * (size + 1));
	if (!heap)
		return (NULL);
	i = 0;
	while (tok && i < size)
	{
		heap[i] = (char *)malloc(sizeof(char) * (strlen(tok->word) + 1));
		if (!heap[i])
		{
			while (i > 0)
				free(heap[--i]);
			free(heap);
			return (NULL);
		}
		strcpy(heap[i], tok->word);
		tok = tok->next;
		i++;
	}
	heap[i] = NULL;
	return (heap);
}

char	**create_argv(t_token *tok)
{
	size_t	size;
	t_token	*tmp;
	char	**argv;

	size = 0;
	tmp = tok;
	while (tmp && tmp->word)
	{
		size++;
		tmp = tmp->next;
	}
	argv = allocate_argv(tok, size);
	return (argv);
}
