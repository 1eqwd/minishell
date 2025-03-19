/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:56:49 by shuu              #+#    #+#             */
/*   Updated: 2025/03/19 14:04:36 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void open_redir_file(t_node *redir)
{
    int flags = 0;
    mode_t mode = 0644;
    if (!redir)
        return ;
    if (redir->kind == ND_REDIR_OUT ||(redir->kind == ND_REDIR_OUT_ERR) || (redir->kind == ND_REDIR_SP))
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    else if (redir->kind == ND_REDIR_IN)
        flags = O_RDONLY;
    else if(redir->kind == ND_REDIR_APPEND_OUT)
        flags = O_WRONLY | O_CREAT | O_APPEND;
    else if (redir->kind == ND_REDIR_APPEND_IN)
        flags = O_RDONLY | O_APPEND;
    if (redir->filename != NULL)
    {
        redir->filefd = open(redir->filename->word, flags, mode);
        // printf("in open_redir_file, filename %s\n", redir->filename->word);
    }
   open_redir_file(redir->next);
}

void do_redirect(t_node *redir)
{
    if (!redir)
        return ;
    if ((redir->kind == ND_REDIR_OUT) || (redir->kind == ND_REDIR_IN) || (redir->kind == ND_REDIR_APPEND_OUT))
    {
        // printf("in do_redirect, redir->filename %s\n", redir->filename->word);
        redir->stashed_targetfd = dup(redir->targetfd);
        // close(redir->filefd);
        dup2(redir->filefd, redir->targetfd);
    }
    else if (redir->kind == ND_REDIR_SP)
    {
        if (!redir->next)
        {
            // printf("in do_redirect, \n");
            redir->stashed_targetfd = dup(redir->targetfd);
            dup2(redir->filefd, redir->targetfd);
        }
    }
    else if (redir->kind == ND_REDIR_OUT_ERR)
    {
        // printf("in do_redirect, redir->filename %s\n", redir->filename->word);
        redir->stashed_targetfd = dup(redir->targetfd);
        redir->stashed_targetfd2 = dup(redir->targetfd);
        dup2(redir->filefd, redir->targetfd);
        dup2(redir->filefd, redir->targetfd2);
    }
    do_redirect(redir->next);
}

void reset_redirect(t_node *redir)
{
    if (!redir)
        return ;
    reset_redirect(redir->next);
    if (redir->kind == ND_REDIR_IN || redir->kind == ND_REDIR_OUT || redir->kind == ND_REDIR_APPEND_OUT || redir->kind == ND_REDIR_APPEND_IN)
    {
        // printf("in reset_direct\n");
        close(redir->filefd);
        close(redir->targetfd);
        dup2(redir->stashed_targetfd, redir->targetfd);
    }
    else if (redir->kind == ND_REDIR_SP)
    {
        // printf("in reset_direct\n");
        if (!redir->next)
        {
            printf("in if\n");
            close(redir->filefd);
            close(redir->targetfd);
            dup2(redir->stashed_targetfd, redir->targetfd);
        }
    }
    else if ((redir->filefd >= 0) && (redir->kind == ND_REDIR_OUT_ERR))
    {
        close(redir->filefd);
        close(redir->targetfd);
        close(redir->targetfd2);
        dup2(redir->stashed_targetfd, redir->targetfd);
        dup2(redir->stashed_targetfd2, redir->targetfd2);
    }
}
