/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:24:52 by retoriya          #+#    #+#             */
/*   Updated: 2025/05/05 15:24:56 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*forkを取る関数*/
void	take_forks(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	// 奇数と偶数でフォークを取る順番を変える（デッドロック防止）
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&data->forks[philo->left_fork].mutex);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&data->forks[philo->right_fork].mutex);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&data->forks[philo->right_fork].mutex);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&data->forks[philo->left_fork].mutex);
		print_status(philo, "has taken a fork");
	}
}

/* フォークを置く関数 */
void	put_forks(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_unlock(&data->forks[philo->left_fork].mutex);
	pthread_mutex_unlock(&data->forks[philo->right_fork].mutex);
}
