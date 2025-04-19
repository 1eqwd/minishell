/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 18:39:47 by mawako            #+#    #+#             */
/*   Updated: 2025/04/19 17:14:22 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_MANAGEMENT_H
# define ENV_MANAGEMENT_H
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

// extern char	**g_env;
typedef struct s_env
{
    char	**g_env;
    char	**environ;
    pid_t	last_bg_pid;
    int		last_exit_status;
}   t_env;

void	init_env(char **envp, t_env *env);
char	*my_getenv(const char *name, t_env *env);
int		my_setenv(const char *name, const char *value, int overwrite, t_env *t_env);
int		my_unsetenv(const char *name, t_env *env);
void	free_env(t_env *env);

#endif
