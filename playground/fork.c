/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sumedai <sumedai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 13:56:53 by sumedai           #+#    #+#             */
/*   Updated: 2025/03/06 15:14:40 by sumedai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) 
{
  pid_t pid;
  int status;

  if ((pid = fork()) < 0)
    perror("fork() error");
  else if (pid == 0) {
    printf("This is the child.\n");
    printf("Child's pid is %d and my parent's is %d\n",
           (int) getpid(), (int) getppid());
    exit(42);
  }
  else {
    printf("This is the parent.\n");
    printf("Parent's pid is %d and my child's is %d\n",
           (int) getpid(), (int) pid);
    printf("I'm waiting for my child to complete.\n");
    if (wait(&status) == -1)
      perror("wait() error");
    else if (WIFEXITED(status))
           printf("The child exited with status of %d\n",
                  WEXITSTATUS(status));
         else
           printf("The child did not exit successfully\n");
  }
  return (0);
}