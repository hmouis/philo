/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmouis <hmouis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:11:47 by hmouis            #+#    #+#             */
/*   Updated: 2025/07/09 14:46:24 by hmouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_status(t_table *table, size_t *i, size_t *last_meal)
{
	pthread_mutex_lock(&table->philo[*i].lock_time);
	*last_meal = table->philo[*i].last_meal;
	pthread_mutex_unlock(&table->philo[*i].lock_time);
	if (full_philos(table))
		return (0);
	if (get_current_time() - *last_meal > table->time_to_die)
	{
		pthread_mutex_lock(&table->dead_lock);
		table->is_dead = true;
		pthread_mutex_unlock(&table->dead_lock);
		return (0);
	}
	(*i)++;
	return (1);
}

void	print_is_dead(t_table *table, size_t last_meal, size_t i)
{
	pthread_mutex_lock(&table->write_lock);
	printf("%zu %zu is dead\n", get_current_time() - last_meal,
		table->philo[i].id);
	pthread_mutex_unlock(&table->write_lock);
}

void	*track_philos(void *data)
{
	t_table	*table;
	size_t	i;
	size_t	last_meal;

	i = 0;
	table = (t_table *)data;
	last_meal = 0;
	while (1)
	{
		i = 0;
		while (i < table->number_of_philos)
		{
			if (full_philos(table))
				break ;
			if (!philo_status(table, &i, &last_meal))
				break ;
		}
		if (full_philos(table))
			break ;
		if (is_dead(table, last_meal, i))
			break ;
		usleep(200);
	}
	return (NULL);
}

int	check_meals_count(t_philo *philo)
{
	pthread_mutex_lock(philo->lock);
	if (philo->table->meals
		&& philo->count_meals == philo->table->number_of_meals)
	{
		pthread_mutex_lock(philo->full_philo);
		philo->full = true;
		pthread_mutex_unlock(philo->full_philo);
		pthread_mutex_unlock(philo->lock);
		return (1);
	}
	pthread_mutex_unlock(philo->lock);
	return (0);
}

void	*dinner(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (1)
	{
		if (check_dead(philo->table))
			break ;
		print_status(philo, "is thinking", 0);
		take_forks(philo);
		count_meals_eating(philo);
		if (check_meals_count(philo))
			break ;
		print_status(philo, "is sleeping", 1);
	}
	return (NULL);
}
