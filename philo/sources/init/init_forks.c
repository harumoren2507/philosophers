#include "includes/philo.h"

void    init_forks(t_data *data)
{
    int i;
    i = 0;
    while (i < data->n_philosophers)
    {
        pthread_mutex_init(&data->forks[i].mutex, NULL);
        i++;
    }
}
