

typedef enum {
    THINKING,
    EATING,
    SLEEPING,
    DEAD,
} t_state;

//void    philosophers(int    number_philo, )

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
