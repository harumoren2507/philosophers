/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:22:12 by retoriya          #+#    #+#             */
/*   Updated: 2025/05/05 15:22:15 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	monitor_thread;

	if (argc != 5 && argc != 6)
	{
		printf("Usage:%s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n",
			argv[0]);
		return (1);
	}
	if (init_simulation(&data, argc, argv))
	{
		printf("Error: invalid arguments or initialization failed\n");
		return (1);
	}
	init_forks(&data);
	init_philosophers(&data);
	if (create_philosopher_threads(&data))
	{
		printf("Error: thread creation failed\n");
		return (1);
	}
	if (pthread_create(&monitor_thread, NULL, death_monitor, &data))
	{
		printf("Error: monitor thread creation failed\n");
		return (1);
	}
	pthread_join(monitor_thread, NULL);
	end_simulation(&data);
	return (0);
}
