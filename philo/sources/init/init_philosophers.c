/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philosophers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:22:55 by retoriya          #+#    #+#             */
/*   Updated: 2025/05/05 15:22:57 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philosophers)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].right_fork = i;
		data->philosophers[i].left_fork = (i + 1) % data->n_philosophers;
		data->philosophers[i].eating_count = 0;
		data->philosophers[i].data = data;
		pthread_mutex_init(&data->philosophers[i].meal_mutex, NULL);
		i++;
	}
}
