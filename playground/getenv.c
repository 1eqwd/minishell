/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sumedai <sumedai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 17:04:12 by sumedai           #+#    #+#             */
/*   Updated: 2025/03/08 13:21:51 by sumedai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* CELEBG05

   In this example, *pathvar points to the value of the PATH
   environment variable.
   In a POSIX environment, this variable would be from the CENV
   group ID.

 */
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
   char *pathvar;

   pathvar = getenv("PATH");
   if (!pathvar)
   {
        printf("%s is not environment variable\n", "SCHOOL");
        exit(1);
   }
   printf("%s\n",pathvar);
   return (0);
}