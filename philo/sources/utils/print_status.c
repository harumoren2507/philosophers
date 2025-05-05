/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:23:49 by retoriya          #+#    #+#             */
/*   Updated: 2025/05/05 15:23:50 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philosopher *philo, char *message)
{
	t_data		*data;
	long long	current_time;
	int			end;

	data = philo->data;
	pthread_mutex_lock(&data->end_mutex);
	end = data->simulation_end;
	pthread_mutex_unlock(&data->end_mutex);
	if (!end)
	{
		current_time = get_time_ms() - data->start_time;
		pthread_mutex_lock(&data->print_mutex);
		printf("%lld %d %s\n", current_time, philo->id, message);
		pthread_mutex_unlock(&data->print_mutex);
	}
}
