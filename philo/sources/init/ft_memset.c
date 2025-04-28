/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 23:20:55 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/31 23:20:58 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void    *ft_memset(void *s, int c, size_t n)
{
    unsigned char *p;

    p = (unsigned char *)s;
    while (n-- > 0)
        *p++ = (unsigned char)c;
    return (s);
}
