#include "includes/philo.h"

void    init_philosophers(t_data *data)
{
    int i;

    i = 0;
    while (i < data->n_philosophers)
    {
        data->philosophers[i].id = i + 1;
        data->philosophers[i].right_fork = i;
        data->philosophers[i].left_fork = (i + 1) % data->n_philosophers;
        data->philosophers[i].eating_count = 0;
        data->philosophers[i].last_meal_time =;
        data->philosophers[i].data = data;
        pthread_mutex_init();
        i++;
    }
}