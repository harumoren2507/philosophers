/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:25:13 by retoriya          #+#    #+#             */
/*   Updated: 2025/05/23 18:08:20 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;
	t_data			*data;

	philo = (t_philosopher *)arg;
	data = philo->data;
	if (data->n_philosophers == 1)
		return (single_philosopher_routine(philo));
	if (philo->id % 2 == 0)
		usleep(5000);
	if (philo->id % 2 != 0)
		precise_sleep(data->time_to_eat / 2);
	while (!simulation_is_over(data))
	{
		philosopher_cycle(philo);
	}
	return (NULL);
}

void	*single_philosopher_routine(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	print_status(philo, "is thinking");
	pthread_mutex_lock(&data->forks[philo->left_fork].mutex);
	print_status(philo, "has taken a fork");
	while (!simulation_is_over(data))
		precise_sleep(1);
	pthread_mutex_unlock(&data->forks[philo->left_fork].mutex);
	return (NULL);
}
