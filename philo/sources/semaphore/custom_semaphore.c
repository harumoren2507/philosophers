/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_semaphore.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:24:22 by retoriya          #+#    #+#             */
/*   Updated: 2025/05/05 15:24:24 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* セマフォの初期化 */
void	sem_custom_init(t_semaphore *sem, int initial_value)
{
	pthread_mutex_init(&sem->mutex, NULL);
	sem->value = initial_value;
}

void	sem_custom_destroy(t_semaphore *sem)
{
	pthread_mutex_destroy(&sem->mutex);
}

void	sem_custom_wait(t_semaphore *sem)
{
	while (1)
	{
		pthread_mutex_lock(&sem->mutex);
		if (sem->value > 0)
		{
			sem->value--;
			pthread_mutex_unlock(&sem->mutex);
			return ;
		}
		pthread_mutex_unlock(&sem->mutex);
		usleep(1000);
	}
}

void	sem_custom_post(t_semaphore *sem)
{
	pthread_mutex_lock(&sem->mutex);
	sem->value++;
	pthread_mutex_unlock(&sem->mutex);
}
