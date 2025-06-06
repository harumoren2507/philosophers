/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_simulation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:24:40 by retoriya          #+#    #+#             */
/*   Updated: 2025/05/05 15:24:42 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	end_simulation(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->end_mutex);
	data->simulation_end = 1;
	pthread_mutex_unlock(&data->end_mutex);
	i = 0;
	while (i < data->n_philosophers)
	{
		sem_custom_post(&data->max_diners);
		usleep(500);
		i++;
	}
	i = 0;
	while (i < data->n_philosophers)
	{
		pthread_join(data->philosophers[i].thread, NULL);
		i++;
	}
	cleanup_resources(data);
	free(data->forks);
	free(data->philosophers);
}
