/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_resources.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:24:34 by retoriya          #+#    #+#             */
/*   Updated: 2025/05/05 15:24:35 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* リソースのクリーンアップの修正版 */
void	cleanup_resources(t_data *data)
{
	int	i;

	// 各哲学者のミューテックスを解放
	i = 0;
	while (i < data->n_philosophers)
	{
		pthread_mutex_destroy(&data->forks[i].mutex);
		pthread_mutex_destroy(&data->philosophers[i].meal_mutex);
		i++;
	}
	// 共有ミューテックスを解放
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->end_mutex);
	pthread_mutex_destroy(&data->meal_check_mutex);
	// 最後にセマフォを破棄
	sem_custom_destroy(&data->max_diners);
}
