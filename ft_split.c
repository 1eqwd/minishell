/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 06:37:23 by mawako            #+#    #+#             */
/*   Updated: 2025/03/26 16:30:37 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == 0))
			count++;
		i++;
	}
	return (count);
}

static size_t	split_words(char **res, char const *s, char c, size_t word)
{
	size_t	i;
	size_t	j;

	j = 0;
	i = 0;
	while (s[j])
	{
		if (s[j] == c || s[j] == 0)
			i = j + 1;
		if (s[j] != c && (s[j + 1] == c || s[j + 1] == 0))
		{
			res[word] = malloc(sizeof(char) * (j - i + 2));
			if (!res[word])
			{
				while (word++)
					free(res[word]);
				return (0);
			}
			strlcpy(res[word], (s + i), j - i + 2);
			word++;
		}
		j++;
	}
	res[word] = 0;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**res;

	if (!s)
		return (0);
	res = malloc(sizeof(char *) * (count(s, c) + 1));
	if (!res)
		return (0);
	if (!split_words(res, s, c, 0))
		return (free(res), NULL);
	return (res);
}
