/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmouis <hmouis@1337.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:19:11 by hmouis            #+#    #+#             */
/*   Updated: 2025/07/03 15:47:39 by hmouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t ft_atoi(char *str)
{
	int i;
	size_t result;

	i = 0;
	result = 0;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}

void ft_usleep(size_t milliseconds)
{
	size_t start;

	start = get_current_time();
	while (get_current_time() - start <= milliseconds)
		usleep(100);
}

int full_philos(t_table *table)
{
	size_t i;

	i = 0;
	while (i < table->number_of_philos)
	{
		if (!table->philo[i].full)
			return (0);
		i++;
	}
	return (1);
}

int check_dead(t_table *table)
{
	pthread_mutex_lock(&table->dead_lock);
	if (table->is_dead)
	{
		pthread_mutex_unlock(&table->dead_lock);
		return (1);
	}
	pthread_mutex_unlock(&table->dead_lock);
	return (0);
}

void take_forks(t_philo *philo)
{
	// if (philo->id % 2 == 0 && philo->count_meals > 0)
	// 	ft_usleep((philo->table->time_to_die - philo->table->time_to_eat - philo->table->time_to_sleep) / 2);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->l_fork);
		if (check_dead(philo->table))
			return ;
		printf("%zu %zu has taken a fork\n", get_current_time() - philo->table->start_time, philo->id);
		pthread_mutex_lock(philo->r_fork);
		if (check_dead(philo->table))
			return ;
		printf("%zu %zu has taken a fork\n", get_current_time() - philo->table->start_time, philo->id);
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		if (check_dead(philo->table))
			return ;
		printf("%zu %zu has taken a fork\n", get_current_time() - philo->table->start_time, philo->id);
		pthread_mutex_lock(philo->l_fork);
		if (check_dead(philo->table))
			return ;
		printf("%zu %zu has taken a fork\n", get_current_time() - philo->table->start_time, philo->id);
	}
}

void count_meals_eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->t_var_lock);
	philo->count_meals++;
	pthread_mutex_unlock(&philo->table->t_var_lock);
}

void get_last_meals_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->lock_time);
	// printf("philo id = [%zu] last meal --------- = %zu\n",philo->id, get_current_time() - philo->last_meal);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->lock_time);
}

void print_status(t_philo *philo, char *status, int flag)
{
	pthread_mutex_lock(&philo->table->dead_lock);
	if (philo->table->is_dead == true)
	{
		pthread_mutex_unlock(&philo->table->dead_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->table->dead_lock);
	pthread_mutex_lock(philo->write_lock);
	printf("%zu %zu is %s\n", get_current_time() - philo->table->start_time, philo->id, status);
	pthread_mutex_unlock(philo->write_lock);
	if (flag == 1)
		ft_usleep(philo->table->time_to_sleep);
	if (flag == 2)
	{
		ft_usleep(philo->table->time_to_eat);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
	}
}
