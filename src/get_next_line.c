/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samercad <samercad@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 22:59:00 by samercad          #+#    #+#             */
/*   Updated: 2026/11/24 22:59:03 by samercad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** Reads BUFFER_SIZE chunks into the stash until it holds a '\n' or EOF.
** Returns the stash, or NULL (with the stash freed) on read/malloc error.
*/
static char	*fill_stash(int fd, char *stash)
{
	char	*buf;
	ssize_t	bytes;

	buf = malloc((size_t)BUFFER_SIZE + 1);
	if (!buf)
	{
		free(stash);
		return (NULL);
	}
	bytes = read(fd, buf, BUFFER_SIZE);
	while (bytes > 0)
	{
		buf[bytes] = '\0';
		stash = ft_strjoin_free(stash, buf);
		if (!stash || ft_strchr(buf, '\n'))
			break ;
		bytes = read(fd, buf, BUFFER_SIZE);
	}
	free(buf);
	if (bytes < 0)
	{
		free(stash);
		return (NULL);
	}
	return (stash);
}

/*
** Splits the stash: returns everything up to and including the first '\n'
** and keeps the rest in *stash (NULL when nothing is left).
*/
static char	*take_line(char **stash)
{
	char	*line;
	char	*rest;
	size_t	len;

	len = ft_strchr(*stash, '\n') - *stash + 1;
	line = ft_substr(*stash, 0, len);
	rest = NULL;
	if (line && (*stash)[len])
	{
		rest = ft_substr(*stash, len, ft_strlen(*stash + len));
		if (!rest)
		{
			free(line);
			line = NULL;
		}
	}
	free(*stash);
	*stash = rest;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!ft_strchr(stash, '\n'))
		stash = fill_stash(fd, stash);
	if (!stash)
		return (NULL);
	if (!ft_strchr(stash, '\n'))
	{
		line = stash;
		stash = NULL;
		return (line);
	}
	return (take_line(&stash));
}
