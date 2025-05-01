/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:34:21 by retoriya          #+#    #+#             */
/*   Updated: 2025/04/30 19:40:51 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

include "includes/philo.h"

int allocate_resources(t_data *data)
{
    data->forks = malloc(sizeof(t_fork) * data->n_philosophers);
    if (!data->forks)
        return (1);
    data->philosophers = malloc(sizeof(t_philosophers) * data->n_philosophers);
    if (!data->philosophers)
    {
      free(data->philosophers);  
      return (1);//ここってfreeする必要ある？
    }
    pthread_mutex_init(&data->print_mutex, NULL);
    pthread_mutex_init(&data->end_mutex, NULL);
    pthread_mutex_init(&data->meal_check_mutex, NULL);
    return (0);
}

int init_simulation(t_data *data, int argc, char **argv)
{
  ft_memset(data, 0, sizeof(t_data));//これをやる意味の説明
  data->n_philosophers = ft_atoi(argv[1]);
  data->time_to_die = ft_atoi(argv[2]);
  data->time_to_eat = ft_atoi(argv[3]);
  data->time_to_sleep = ft_atoi(argv[4]);
  data->simulation_end = 0;
  if (argc == 6)
    data->n_must_eat = ft_atoi(argv[5]);
  else
    data->n_must_eat = -1;//無限に食べ続ける
  if (data->n_philosophers <= 0 || data->time_to_die < 60 ||
    data->time_to_eat < 60 || data->time_to_sleep < 60 
    || (argc == 6 && data->n_must_eat <= 0))
    return (1);
  return (allocate_resources(data));
}
