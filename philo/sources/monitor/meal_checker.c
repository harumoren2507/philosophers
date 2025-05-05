/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meal_checker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:23:20 by retoriya          #+#    #+#             */
/*   Updated: 2025/05/05 15:23:21 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* 全ての哲学者が十分に食事したかチェック */
int	all_ate_enough(t_data *data)
{
	int	i;
	int	result;

	result = 1;
	if (data->n_must_eat < 0)
		return (0);
	pthread_mutex_lock(&data->meal_check_mutex);
	i = 0;
	while (i < data->n_philosophers)
	{
		if (data->philosophers[i].eating_count < data->n_must_eat)
		{
			result = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&data->meal_check_mutex);
	return (result);
}
