/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:07:41 by retoriya          #+#    #+#             */
/*   Updated: 2025/05/05 16:07:42 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_semaphore
{
	pthread_mutex_t	mutex;
	pthread_mutex_t	block;
	int				value;
}					t_semaphore;

typedef struct s_philosopher
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				eating_count;
	long long		last_meal_time;
	pthread_mutex_t	meal_mutex;
	struct s_data	*data;
	pthread_t		thread;
}					t_philosopher;

typedef struct s_fork
{
	pthread_mutex_t	mutex;
}					t_fork;

typedef struct s_data
{
	int				n_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				n_must_eat;
	int				simulation_end;
	long long		start_time;
	t_fork			*forks;
	t_philosopher	*philosophers;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	end_mutex;
	pthread_mutex_t	meal_check_mutex;
	t_semaphore		max_diners;
}					t_data;

int					ft_atoi(const char *str);
void				*ft_memset(void *s, int c, size_t n);
int					init_simulation(t_data *data, int argc, char **argv);
void				*ft_memset(void *s, int c, size_t n);
void				init_philosophers(t_data *data);
void				init_forks(t_data *data);
int					init_components(t_data *data);
int					start_simulation(t_data *data, pthread_t *monitor_thread);
int					allocate_resources(t_data *data);
int					create_philosopher_threads(t_data *data);
void				*philosopher_routine(void *arg);
void				*single_philosopher_routine(t_philosopher *philo);
void				sem_custom_wait(t_semaphore *sem);
void				sem_custom_post(t_semaphore *sem);
void				sem_custom_init(t_semaphore *sem, int initial_value);
void				sem_custom_destroy(t_semaphore *sem);
void				take_forks(t_philosopher *philo);
void				put_forks(t_philosopher *philo);
void				eat(t_philosopher *philo);
void				philosopher_cycle(t_philosopher *philo);
void				update_last_meal_time(t_philosopher *philo);
long long			get_last_meal_time(t_philosopher *philo);
void				*death_monitor(void *arg);
void				death_monitor_loop(t_data *data);
int					check_philosopher_death(t_data *data, int i);
void				end_simulation(t_data *data);
void				cleanup_resources(t_data *data);
void				precise_sleep(int ms);
long long			get_time_ms(void);
void				print_status(t_philosopher *philo, char *message);
int					simulation_is_over(t_data *data);
int					all_ate_enough(t_data *data);
#endif
