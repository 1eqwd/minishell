/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sumedai <sumedai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 13:57:19 by sumedai           #+#    #+#             */
/*   Updated: 2025/03/08 15:31:27 by sumedai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        char *argv[] = {"/bin/echo", NULL};
        int num = execve("/bin/echo", argv, NULL);
        if (num < 0)
            strerror(num);
    }
    else if (pid > 0)
    {
        (void)pid;
    }
    else
    {
        printf("Error:failure fork\n");
    }
    return (0);
}
