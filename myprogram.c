/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myprogram.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:42:51 by shuu              #+#    #+#             */
/*   Updated: 2025/03/19 14:07:48 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(void)
{
    // int fd = open("test.log" ,O_WRONLY | O_CREAT | O_TRUNC);
    // printf("%d\n" ,fd);
    write(1, "hello\n", 6);
    write(2, "error\n", 6);
    return (0);
}