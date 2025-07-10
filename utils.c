/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmouis <hmouis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:19:11 by hmouis            #+#    #+#             */
/*   Updated: 2025/07/10 16:40:30 by hmouis           ###   ########.fr       */
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
	while (get_current_time() - start < milliseconds)
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
