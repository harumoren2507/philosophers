/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_creation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:23:33 by retoriya          #+#    #+#             */
/*   Updated: 2025/05/05 15:23:38 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_philosopher_threads(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	data->start_time = get_time_ms() + 10;
	while (i < data->n_philosophers)
	{
		data->philosophers[i].last_meal_time = data->start_time;
		i++;
	}
	while (j < data->n_philosophers)
	{
		if (pthread_create(&data->philosophers[j].thread, NULL,
				philosopher_routine, &data->philosophers[j]))
			return (1);
		j++;
	}
	return (0);
}
