/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmouis <hmouis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:19:11 by hmouis            #+#    #+#             */
/*   Updated: 2025/07/09 01:38:01 by hmouis           ###   ########.fr       */
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

void	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while (get_current_time() - start <= milliseconds)
		usleep(100);
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

int	check_dead(t_table *table)
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
