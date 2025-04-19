/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:22:06 by mawako            #+#    #+#             */
/*   Updated: 2025/04/19 16:01:17 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_management.h"

// char	**g_env = NULL;

void	init_env(char **envp, t_env *env)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (envp[count])
		count++;
	env->g_env = malloc((count + 1) * sizeof(char *));
	if (!env->g_env)
	{
		perror("malloc");
		exit(1);
	}
	while (i < count)
	{
		env->g_env[i] = strdup(envp[i]);
		if (!env->g_env[i++])
		{
			perror("strdup");
			exit(1);
		}
	}
	env->g_env[count] = (NULL);
}

char	*my_getenv(const char *name, t_env *env)
{
	int		i;
	size_t	len;

	i = 0;
	if (!name || !env->g_env)
		return (NULL);
	len = strlen(name);
	while (env->g_env[i])
	{
		if (strncmp(env->g_env[i], name, len) == 0 && env->g_env[i][len] == '=')
			return (env->g_env[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	my_setenv(const char *name, const char *value, int overwrite, t_env *env)
{
	int		i;
	size_t	len;
	char	*new_entry;
	int		count;
	char	**new_env;

	i = 0;
	count = 0;
	if (!name || strchr(name, '='))
		return (-1);
	len = strlen(name);
	if (env->g_env)
	{
		while (env->g_env[i])
		{
			count++;
			if (strncmp(env->g_env[i], name, len) == 0 && env->g_env[i][len] == '=')
			{
				if (!overwrite)
					return (0);
				new_entry = malloc(len + strlen(value) + 2);
				if (!new_entry)
					return (-1);
				sprintf(new_entry, "%s=%s", name, value);
				free(env->g_env[i]);
				env->g_env[i] = new_entry;
				return (0);
			}
			i++;
		}
	}
	{
		new_env = realloc(env->g_env, (count + 2) * sizeof(char *));
		if (!new_env)
			return (-1);
		env->g_env = new_env;
		new_entry = malloc(len + strlen(value) + 2);
		if (!new_entry)
			return (-1);
		sprintf(new_entry, "%s=%s", name, value);
		env->g_env[count] = new_entry;
		env->g_env[count + 1] = NULL;
		return (0);
	}
}

int	my_unsetenv(const char *name, t_env *env)
{
	int		i;
	int		j;
	size_t	len;

	i = 0;
	j = 0;
	if (!name || !env->g_env)
		return (-1);
	len = strlen(name);
	while (env->g_env[i])
	{
		if (strncmp(env->g_env[i], name, len) == 0 && env->g_env[i][len] == '=')
		{
			free(env->g_env[i]);
			while (env->g_env[j])
			{
				env->g_env[j] = env->g_env[j + 1];
				j++;
			}
			i--;
		}
	}
	return (0);
}

void	free_env(t_env *env)
{
	int	i;

	i = 0;
	if (!env->g_env)
		return ;
	while (env->g_env[i])
		free(env->g_env[i++]);
	free(env->g_env);
	env->g_env = (NULL);
}
