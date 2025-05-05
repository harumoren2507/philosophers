/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_is_over.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:23:54 by retoriya          #+#    #+#             */
/*   Updated: 2025/05/05 15:23:55 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	simulation_is_over(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->end_mutex);
	result = data->simulation_end;
	pthread_mutex_unlock(&data->end_mutex);
	return (result);
}
