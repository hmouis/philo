/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmouis <hmouis@1337.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:29:37 by hmouis            #+#    #+#             */
/*   Updated: 2025/07/03 15:47:51 by hmouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include <sys/time.h>

typedef struct s_table t_table;

typedef struct s_philo
{
	size_t last_meal;
	size_t id;
	pthread_mutex_t *l_fork;
	pthread_mutex_t *r_fork;
	size_t count_meals;
	pthread_t thread;
	pthread_mutex_t lock_time;
	bool full;
	pthread_mutex_t *write_lock;
	pthread_mutex_t *lock;
	pthread_mutex_t *var_lock;
	t_table *table;
}				t_philo;

typedef struct s_table
{
	size_t number_of_philos;
	size_t time_to_die;
	size_t number_of_meals;
	size_t time_to_sleep;
	size_t time_to_eat;
	size_t start_time;
	pthread_t *all_thread;
	pthread_t t_th;
	bool meals;
	bool is_dead;
	pthread_mutex_t *forks;
	pthread_mutex_t write_lock;
	pthread_mutex_t lock;
	pthread_mutex_t var_lock;
	pthread_mutex_t t_var_lock;
	pthread_mutex_t dead_lock;
	t_philo *philo;
}			t_table;


void print_status(t_philo *philo, char *status, int flag);
int full_philos(t_table *table);
void *track_philos(void *data);
size_t	get_current_time(void);
int pars_args(int ac, char **av);
size_t ft_atoi(char *str);
void init_philos(t_table *table, char **av);
void *dinner(void *data);




# endif
