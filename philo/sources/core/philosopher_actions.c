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

/* 食事を行う関数 */
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

/* 哲学者のメインサイクル関数 */
void	philosopher_cycle(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	// 思考
	print_status(philo, "is thinking");
	// 同時に食事できる哲学者数を制限（セマフォを使用）
	sem_custom_wait(&data->max_diners);
	// フォークの取得
	take_forks(philo);
	// 食事
	eat(philo);
	// フォークを解放
	put_forks(philo);
	// セマフォを解放（他の哲学者が食事できるように）
	sem_custom_post(&data->max_diners);
	// 睡眠（シミュレーション終了チェック付き）
	if (!simulation_is_over(data))
	{
		print_status(philo, "is sleeping");
		precise_sleep(data->time_to_sleep);
	}
}

/* 哲学者の最後の食事時間を更新*/
void	update_last_meal_time(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&philo->meal_mutex);
}

/* 哲学者の最後の食事時間を取得 */
long long	get_last_meal_time(t_philosopher *philo)
{
	long long	time;

	pthread_mutex_lock(&philo->meal_mutex);
	time = philo->last_meal_time;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (time);
}
