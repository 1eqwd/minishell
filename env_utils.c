/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:10:12 by mawako            #+#    #+#             */
/*   Updated: 2025/04/19 17:01:43 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(const char *name, t_env *env)
{
	// extern char	**environ;
	int			i;
	int			len;

	len = strlen(name);
	i = 0;
	while (env->environ[i])
	{
		if (strncmp(env->environ[i], name, len) == 0 && env->environ[i][len] == '=')
			return (env->environ[i] + len + 1);
		i++;
	}
	return ("");
}

char	*get_shell_pid_str(void)
{
	int		fd;
	char	buf[256];
	int		n;
	int		i;
	char	*pid_str;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
		return (strdup("0"));
	n = read(fd, buf, 255);
	if (n <= 0)
	{
		close(fd);
		return (strdup("0"));
	}
	buf[n] = '\0';
	close(fd);
	i = 0;
	while (buf[i] && buf[i] != ' ')
		i++;
	buf[i] = '\0';
	pid_str = strdup(buf);
	return (pid_str);
}
