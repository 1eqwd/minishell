/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:03:45 by mawako            #+#    #+#             */
/*   Updated: 2025/04/19 17:22:08 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// extern char	**environ;
// extern char	**g_env;

static int	is_valid_echo_option(const char *str)
{
	int	i;

	if (!str || str[0] != '-' || !str[1])
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n' && str[i] != 'e' && str[i] != 'E')
			return (0);
		i++;
	}
	return (1);
}

static void	update_env(const char *key, const char *value, t_env *env)
{
	int		key_len;
	int		i;
	char	*new_entry;
	char	**new_env;
	int		count;

	key_len = strlen(key);
	i = 0;
	while (env->g_env && env->g_env[i])
	{
		if (strncmp(env->g_env[i], key, key_len) == 0 && env->g_env[i][key_len] == '=')
		{
			new_entry = malloc(key_len + 1 + strlen(value) + 1);
			if (!new_entry)
				fatal_error("malloc failed in update_env");
			sprintf(new_entry, "%s=%s", key, value);
			free(env->g_env[i]);
			env->g_env[i] = new_entry;
			env->environ = env->g_env;
			return ;
		}
		i++;
	}
	count = i;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		fatal_error("malloc failed in update_env");
	i = 0;
	while (i < count)
	{
		new_env[i] = env->g_env[i];
		i++;
	}
	new_entry = malloc(key_len + 1 + strlen(value) + 1);
	if (!new_entry)
		fatal_error("malloc failed in update_env");
	sprintf(new_entry, "%s=%s", key, value);
	new_env[count] = new_entry;
	new_env[count + 1] = NULL;
	if (env->g_env)
		free(env->g_env);
	env->g_env = new_env;
	env->environ = env->g_env;
}

static void	remove_env(const char *key, t_env *env)
{
	int		key_len;
	int		i;
	int		j;
	char	**new_env;
	int		count;
	int		index_to_remove;

	key_len = strlen(key);
	count = 0;
	i = 0;
	while (env->g_env && env->g_env[i])
	{
		count++;
		i++;
	}
	index_to_remove = -1;
	i = 0;
	while (env->g_env && env->g_env[i])
	{
		if (strncmp(env->g_env[i], key, key_len) == 0 && env->g_env[i][key_len] == '=')
		{
			index_to_remove = i;
			break ;
		}
		i++;
	}
	if (index_to_remove == -1)
		return ;
	free(env->g_env[index_to_remove]);
	new_env = malloc(sizeof(char *) * (count));
	if (!new_env)
		fatal_error("malloc failed in remove_env");
	i = 0;
	j = 0;
	while (env->g_env[i])
	{
		if (i == index_to_remove)
		{
			i++;
			continue ;
		}
		new_env[j++] = env->g_env[i];
		i++;
	}
	new_env[j] = NULL;
	free(env->g_env);
	env->g_env = new_env;
	env->environ = env->g_env;
}

static char	*interpret_escapes(const char *str)
{
	size_t	len;
	char	*buf;
	size_t	i;
	size_t	j;

	len = strlen(str);
	buf = malloc(len * 2 + 1);
	i = 0;
	j = 0;
	if (!buf)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\')
		{
			i++;
			if (str[i] == 'n')
				buf[j++] = '\n';
			else if (str[i] == 't')
				buf[j++] = '\t';
			else if (str[i] == 'r')
				buf[j++] = '\r';
			else if (str[i] == '\\')
				buf[j++] = '\\';
			else
			{
				buf[j++] = '\\';
				if (str[i])
					buf[j++] = str[i];
			}
			if (str[i])
				i++;
		}
		else
			buf[j++] = str[i++];
	}
	buf[j] = '\0';
	return (buf);
}

static int	builtin_echo(char **argv)
{
	int		i;
	int		j;
	int		newline;
	int		interpret;
	char	*processed;

	i = 1;
	newline = 1;
	interpret = 0;
	if (argv[1] && is_valid_echo_option(argv[1]))
	{
		j = 1;
		while (argv[1][j])
		{
			if (argv[1][j] == 'n')
				newline = 0;
			else if (argv[1][j] == 'e')
				interpret = 1;
			j++;
		}
		i++;
	}
	while (argv[i])
	{
		if (interpret)
		{
			processed = interpret_escapes(argv[i]);
			printf("%s", processed);
			free(processed);
		}
		else
			printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

static int	builtin_cd(char **argv)
{
	char	*home;

	if (!argv[1])
	{
		home = getenv("HOME");
		if (!home)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
		if (chdir(home) != 0)
		{
			perror("cd");
			return (1);
		}
		return (0);
	}
	if (chdir(argv[1]) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

static int	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

static int	builtin_env(char **argv, t_env *env)
{
	int	i;

	i = 0;
	(void)argv;
	while (env->environ[i])
	{
		printf("%s\n", env->environ[i]);
		i++;
	}
	return (0);
}

static int	builtin_export(char **argv, t_env *env)
{
	int		i;
	int		j;
	char	*key_val;
	char	*eq;
	char	*val;

	if (!argv[1])
	{
		j = 0;
		while (env->g_env && env->g_env[j])
		{
			printf("declare -x %s\n", env->g_env[j]);
			j++;
		}
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		key_val = argv[i];
		eq = strchr(key_val, '=');
		if (!eq)
		{
			val = getenv(key_val);
			if (!val)
				val = "";
			update_env(key_val, val, env);
		}
		else
		{
			*eq = '\0';
			update_env(key_val, eq + 1, env);
			*eq = '=';
		}
		i++;
	}
	return (0);
}

static int	builtin_unset(char **argv, t_env *env)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		remove_env(argv[i], env);
		unsetenv(argv[i]);
		i++;
	}
	env->environ = env->g_env;
	return (0);
}

static int	builtin_exit(char **argv)
{
	int	exit_status;

	exit_status = 0;
	fprintf(stderr, "exit\n");
	if (argv[1])
		exit_status = atoi(argv[1]);
	exit(exit_status);
	return (exit_status);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!strcmp(cmd, "echo"))
		return (1);
	if (!strcmp(cmd, "cd"))
		return (1);
	if (!strcmp(cmd, "pwd"))
		return (1);
	if (!strcmp(cmd, "env"))
		return (1);
	if (!strcmp(cmd, "export"))
		return (1);
	if (!strcmp(cmd, "unset"))
		return (1);
	if (!strcmp(cmd, "exit"))
		return (1);
	return (0);
}

int	exec_builtin(char **argv, t_env *env)
{
	if (!argv[0])
		return (1);
	if (!strcmp(argv[0], "echo"))
		return (builtin_echo(argv));
	else if (!strcmp(argv[0], "cd"))
		return (builtin_cd(argv));
	else if (!strcmp(argv[0], "pwd"))
		return (builtin_pwd());
	else if (!strcmp(argv[0], "env"))
		return (builtin_env(argv, env));
	else if (!strcmp(argv[0], "export"))
		return (builtin_export(argv, env));
	else if (!strcmp(argv[0], "unset"))
		return (builtin_unset(argv, env));
	else if (!strcmp(argv[0], "exit"))
		return (builtin_exit(argv));
	return (1);
}
