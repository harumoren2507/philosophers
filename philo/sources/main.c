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
		printf("Usage: %s <philos> <die> <eat> <sleep> [meals]\n", argv[0]);
		return (1);
	}
	if (init_simulation(&data, argc, argv) || init_components(&data))
	{
		printf("Error: invalid arguments or initialization failed\n");
		return (1);
	}
	if (start_simulation(&data, &monitor_thread))
	{
		printf("Error: simulation start failed\n");
	}
	pthread_join(monitor_thread, NULL);
	end_simulation(&data);
	return (0);
}
