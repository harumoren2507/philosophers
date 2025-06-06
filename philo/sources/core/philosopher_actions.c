/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:25:04 by retoriya          #+#    #+#             */
/*   Updated: 2025/05/05 15:25:05 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	if (!simulation_is_over(data))
	{
		print_status(philo, "is eating");
		update_last_meal_time(philo);
		precise_sleep(data->time_to_eat);
		pthread_mutex_lock(&data->meal_check_mutex);
		philo->eating_count++;
		pthread_mutex_unlock(&data->meal_check_mutex);
	}
}

void	philosopher_cycle(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	print_status(philo, "is thinking");
	sem_custom_wait(&data->max_diners);
	take_forks(philo);
	eat(philo);
	put_forks(philo);
	sem_custom_post(&data->max_diners);
	if (!simulation_is_over(data))
	{
		print_status(philo, "is sleeping");
		precise_sleep(data->time_to_sleep);
	}
}

void	update_last_meal_time(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&philo->meal_mutex);
}

long long	get_last_meal_time(t_philosopher *philo)
{
	long long	time;

	pthread_mutex_lock(&philo->meal_mutex);
	time = philo->last_meal_time;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (time);
}
