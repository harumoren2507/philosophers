/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:24:05 by retoriya          #+#    #+#             */
/*   Updated: 2025/05/05 15:24:07 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/* 指定されたミリ秒だけ高精度に待機 */
void	precise_sleep(int ms)
{
	long long	start;
	long long	current;

	start = get_time_ms();
	while (1)
	{
		current = get_time_ms();
		if (current - start >= ms)
			break ;
		usleep(100);
	}
}
