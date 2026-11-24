/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samercad <samercad@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 22:58:54 by samercad          #+#    #+#             */
/*   Updated: 2026/11/24 22:58:55 by samercad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s && *s != (char)c)
		s++;
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}

/*
** Returns stash + buf in a new string and frees the old stash
** (also on failure, so the caller never leaks it).
*/
char	*ft_strjoin_free(char *stash, const char *buf)
{
	char	*joined;
	size_t	s_len;
	size_t	b_len;
	size_t	i;

	s_len = ft_strlen(stash);
	b_len = ft_strlen(buf);
	joined = malloc(s_len + b_len + 1);
	if (joined)
	{
		i = -1;
		while (++i < s_len)
			joined[i] = stash[i];
		i = -1;
		while (++i < b_len)
			joined[s_len + i] = buf[i];
		joined[s_len + b_len] = '\0';
	}
	free(stash);
	return (joined);
}

/* Caller guarantees s has at least start + len characters. */
char	*ft_substr(const char *s, size_t start, size_t len)
{
	char	*sub;
	size_t	i;

	sub = malloc(len + 1);
	if (!sub)
		return (NULL);
	i = -1;
	while (++i < len)
		sub[i] = s[start + i];
	sub[len] = '\0';
	return (sub);
}
