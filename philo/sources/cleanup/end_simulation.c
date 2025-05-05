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

/* シミュレーションの終了処理の修正版 */
void	end_simulation(t_data *data)
{
	int	i;

	// シミュレーション終了フラグを確実にセット
	pthread_mutex_lock(&data->end_mutex);
	data->simulation_end = 1;
	pthread_mutex_unlock(&data->end_mutex);
	// セマフォで待機中のスレッドを解放
	i = 0;
	while (i < data->n_philosophers)
	{
		sem_custom_post(&data->max_diners);
		usleep(500); // スレッドがセマフォを処理する時間を確保
		i++;
	}
	// スレッドの終了を待つ
	i = 0;
	while (i < data->n_philosophers)
	{
		pthread_join(data->philosophers[i].thread, NULL);
		i++;
	}
	// リソースのクリーンアップと解放
	cleanup_resources(data);
	free(data->forks);
	free(data->philosophers);
}
