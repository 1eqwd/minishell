/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:05:46 by mawako            #+#    #+#             */
/*   Updated: 2025/03/28 17:19:45 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_redir_file(t_redirect *redir)
{
	int		flags;
	mode_t	mode;

	while (redir)
	{
		flags = 0;
		mode = 0644;
		if (redir->type == ND_REDIR_OUT
			|| redir->type == ND_REDIR_OUT_ERR
			|| redir->type == ND_REDIR_SP)
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		else if (redir->type == ND_REDIR_IN || redir->type == R_HEREDOC)
			flags = O_RDONLY;
		else if (redir->type == ND_REDIR_APPEND_OUT)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else if (redir->type == ND_REDIR_APPEND_IN)
			flags = O_RDONLY | O_APPEND;
		if (redir->type == R_HEREDOC)
			redir->filefd = open(redir->heredoc_filename, O_RDONLY);
		else if (redir->word)
			redir->filefd = open(redir->word, flags, mode);
		redir = redir->next;
	}
}

void	do_redirect(t_redirect *redir)
{
	while (redir)
	{
		if (redir->type == ND_REDIR_OUT
			|| redir->type == ND_REDIR_IN
			|| redir->type == ND_REDIR_APPEND_OUT)
		{
			redir->stashed_targetfd = dup(redir->targetfd);
			dup2(redir->filefd, redir->targetfd);
		}
		else if (redir->type == ND_REDIR_SP && !redir->next)
		{
			redir->stashed_targetfd = dup(redir->targetfd);
			dup2(redir->filefd, redir->targetfd);
		}
		else if (redir->type == ND_REDIR_OUT_ERR)
		{
			redir->stashed_targetfd = dup(redir->targetfd);
			redir->stashed_targetfd2 = dup(redir->targetfd2);
			dup2(redir->filefd, redir->targetfd);
			dup2(redir->filefd, redir->targetfd2);
		}
		else if (redir->type == R_HEREDOC)
		{
			redir->stashed_targetfd = dup(redir->targetfd);
			dup2(redir->filefd, redir->targetfd);
		}
		redir = redir->next;
	}
}

void	reset_redirect(t_redirect *redir)
{
	while (redir)
	{
		if (redir->type == ND_REDIR_IN || redir->type == ND_REDIR_OUT
			|| redir->type == ND_REDIR_APPEND_OUT || redir->type == ND_REDIR_APPEND_IN
			|| redir->type == R_HEREDOC)
		{
			close(redir->filefd);
			dup2(redir->stashed_targetfd, redir->targetfd);
			close(redir->stashed_targetfd);
		}
		else if (redir->type == ND_REDIR_SP && !redir->next)
		{
			close(redir->filefd);
			dup2(redir->stashed_targetfd, redir->targetfd);
			close(redir->stashed_targetfd);
		}
		else if (redir->type == ND_REDIR_OUT_ERR && redir->filefd >= 0)
		{
			close(redir->filefd);
			dup2(redir->stashed_targetfd, redir->targetfd);
			dup2(redir->stashed_targetfd2, redir->targetfd2);
			close(redir->stashed_targetfd);
			close(redir->stashed_targetfd2);
		}
		redir = redir->next;
	}
}
