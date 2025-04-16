#include <stdlib.h>
#include <pthread.h>

typedef struct s_philosopher
{
    int last_meal_time;
    int number_of_meal;
    pthread_t thread;
}t_philopher;
typedef struct s_shared_data
{
    int number_philo;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;

}t_shared_data;


typedef enum {
    THINKING,
    EATING,
    SLEEPING,
    DEAD,
} t_state;

void    *philosopher_routine(void   *arg)
{
    t_philopher *philo;

    philo = (t_philopher *)arg;
    while (/*!check_death(philo)*/)
    {
        //think(philo);
        //take_fork(philo);
        //eat(philo);
        //put_fork(philo);
        //sleep(philo);
    }
    return (NULL);
}

#include <stdlib.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
    if (argc < 5 || argc > 6)
    {
        printf("使用法: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return (1);
    }
    int number_of_philosophers = atoi(argv[1]);
    int time_to_die = atoi(argv[2]);
    int time_to_eat = atoi(argv[3]);
    int time_to_sleep = atoi(argv[4]);
    printf("哲学者の数: %d\n", number_of_philosophers);
    return (0);
}
