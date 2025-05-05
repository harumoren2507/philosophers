/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:23:14 by retoriya          #+#    #+#             */
/*   Updated: 2025/05/05 15:23:15 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* 死亡したフィロソファーをチェック */
int	check_philosopher_death(t_data *data, int i)
{
	long long	current_time;
	long long	last_meal;

	current_time = get_time_ms();
	last_meal = get_last_meal_time(&data->philosophers[i]);
	if (current_time - last_meal > data->time_to_die)
	{
		print_status(&data->philosophers[i], "died");
		pthread_mutex_lock(&data->end_mutex);
		data->simulation_end = 1;
		pthread_mutex_unlock(&data->end_mutex);
		return (1);
	}
	return (0);
}

/* 哲学者の死亡監視のループ処理の修正版 */
void	death_monitor_loop(t_data *data)
{
	int	i;

	usleep(5000);
	while (!simulation_is_over(data))
	{
		i = 0;
		while (i < data->n_philosophers && !simulation_is_over(data))
		{
			if (check_philosopher_death(data, i))
				break ;
			i++;
		}
		if (all_ate_enough(data) && !simulation_is_over(data))
		{
			pthread_mutex_lock(&data->end_mutex);
			data->simulation_end = 1;
			pthread_mutex_unlock(&data->end_mutex);
			pthread_mutex_lock(&data->print_mutex);
			printf("All philosophers have eaten enough.\n");
			pthread_mutex_unlock(&data->print_mutex);
			break ;
		}
		usleep(1000);
	}
}

/* 哲学者の死亡監視 */
void	*death_monitor(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	precise_sleep(1);
	death_monitor_loop(data);
	return (NULL);
}
