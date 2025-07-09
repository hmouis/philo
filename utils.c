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
		pthread_mutex_lock(table->philo[i].full_philo);
		if (!table->philo[i].full)
		{
			pthread_mutex_unlock(table->philo[i].full_philo);
			return (0);
		}
		pthread_mutex_unlock(table->philo[i].full_philo);
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
	if (philo->table->number_of_philos % 2 && philo->count_meals > 0)
		ft_usleep((philo->table->time_to_die - philo->table->time_to_eat - philo->table->time_to_sleep) / 2);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->l_fork);
		ft_printf(philo, "has taken a fork");
		pthread_mutex_lock(philo->r_fork);
		ft_printf(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		ft_printf(philo, "has taken a fork");
		pthread_mutex_lock(philo->l_fork);
		ft_printf(philo, "has taken a fork");
	}
	get_last_meals_time(philo);
	print_status(philo, "is eating", 2);
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
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->lock_time);
}

void ft_printf(t_philo *philo, char *status)
{
	static int stop_printing;
	
	pthread_mutex_lock(&philo->table->dead_lock);
	if (philo->table->is_dead == true)
		stop_printing = 1;
	pthread_mutex_unlock(&philo->table->dead_lock);
	pthread_mutex_lock(philo->write_lock);
	if (!stop_printing)
		printf("%zu %zu %s\n", get_current_time() - philo->table->start_time, philo->id, status);
	pthread_mutex_unlock(philo->write_lock);
}

void print_status(t_philo *philo, char *status, int flag)
{
	ft_printf(philo, status);
	if (flag == 1)
		ft_usleep(philo->table->time_to_sleep);
	if (flag == 2)
	{
		ft_usleep(philo->table->time_to_eat);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
	}
}
