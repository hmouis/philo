/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmouis <hmouis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:19:11 by hmouis            #+#    #+#             */
/*   Updated: 2025/07/11 15:15:19 by hmouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_atoi(char *str)
{
	int		i;
	size_t	result;

	i = 0;
	result = 0;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}

void	ft_usleep(size_t milliseconds, t_philo *philo)
{
	size_t	start;

	start = get_current_time();
	while (get_current_time() - start < milliseconds)
	{
		pthread_mutex_lock(&philo->table->dead_lock);
		if (philo->table->is_dead)
		{
			pthread_mutex_unlock(&philo->table->dead_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->table->dead_lock);
		usleep(100);
	}
}

int	full_philos(t_table *table)
{
	size_t	i;

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

void	*philo_one(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	printf("%zu 1 has taken a fork\n", philo->last_meal
		- philo->table->start_time);
	ft_usleep(philo->table->time_to_die, philo);
	philo->last_meal = get_current_time();
	printf("%zu 1 is dead\n", philo->last_meal - philo->table->start_time);
	return (NULL);
}

void	one_philo(t_table *table)
{
	if (pthread_create(&table->all_thread[0], NULL, philo_one,
			&table->philo[0]))
		return ;
	if (pthread_join(table->all_thread[0], NULL))
		return ;
}
