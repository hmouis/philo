/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmouis <hmouis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:29:37 by hmouis            #+#    #+#             */
/*   Updated: 2025/07/10 17:13:15 by hmouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_table	t_table;

typedef struct s_philo
{
	size_t				last_meal;
	size_t				id;
	pthread_mutex_t		*l_fork;
	pthread_mutex_t		*r_fork;
	size_t				count_meals;
	pthread_t			thread;
	pthread_mutex_t		lock_time;
	bool				full;
	pthread_mutex_t		*write_lock;
	pthread_mutex_t		*lock;
	pthread_mutex_t		*var_lock;
	pthread_mutex_t		*full_philo;
	t_table				*table;
}						t_philo;

typedef struct s_table
{
	size_t				number_of_philos;
	size_t				time_to_die;
	size_t				number_of_meals;
	size_t				time_to_sleep;
	size_t				time_to_eat;
	size_t				start_time;
	pthread_t			*all_thread;
	pthread_t			*t_th;
	bool				meals;
	bool				is_dead;
	pthread_mutex_t		*forks;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		lock;
	pthread_mutex_t		var_lock;
	pthread_mutex_t		t_var_lock;
	pthread_mutex_t		full_philo;
	pthread_mutex_t		dead_lock;
	pthread_mutex_t		print_mutex;
	t_philo				*philo;
}						t_table;

void					free_all(t_table *table);
void					print_status(t_philo *philo, char *status, int flag);
int						full_philos(t_table *table);
void					*track_philos(void *data);
size_t					get_current_time(void);
int						pars_args(int ac, char **av);
size_t					ft_atoi(char *str);
int						init_philos(t_table *table, char **av);
void					*dinner(void *data);
void					ft_printf(t_philo *philo, char *str);
void					print_status(t_philo *philo, char *status, int flag);
int						full_philos(t_table *table);
void					ft_usleep(size_t milliseconds);
void					take_forks(t_philo *philo);
void					count_meals_eating(t_philo *philo);
void					get_last_meals_time(t_philo *philo);
void					print_is_dead(t_table *table, size_t last_meal,
							size_t i);

#endif
