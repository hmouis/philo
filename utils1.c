/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmouis <hmouis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 01:37:36 by hmouis            #+#    #+#             */
/*   Updated: 2025/07/11 15:48:12 by hmouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	long	time;

	if (philo->table->number_of_philos % 2 && philo->count_meals > 0)
	{
		time = (philo->table->time_to_die - philo->table->time_to_eat
				- philo->table->time_to_sleep);
		if (time > 0)
			ft_usleep(time / 2, philo);
	}
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
	print_status(philo, "is eating", 2);
}

void	count_meals_eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->t_var_lock);
	philo->count_meals++;
	pthread_mutex_unlock(&philo->table->t_var_lock);
}

void	get_last_meals_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->lock_time);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->lock_time);
}

void	ft_printf(t_philo *philo, char *status)
{
	static int	stop_printing;

	pthread_mutex_lock(&philo->table->dead_lock);
	if (philo->table->is_dead == true)
		stop_printing = 1;
	pthread_mutex_unlock(&philo->table->dead_lock);
	pthread_mutex_lock(philo->write_lock);
	pthread_mutex_lock(&philo->table->dead_lock);
	if (!stop_printing)
		printf("%zu %zu %s\n", get_current_time() - philo->table->start_time,
			philo->id, status);
	pthread_mutex_unlock(&philo->table->dead_lock);
	pthread_mutex_unlock(philo->write_lock);
}

void	print_status(t_philo *philo, char *status, int flag)
{
	ft_printf(philo, status);
	if (flag == 1)
		ft_usleep(philo->table->time_to_sleep, philo);
	if (flag == 2)
	{
		get_last_meals_time(philo);
		ft_usleep(philo->table->time_to_eat, philo);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
	}
}
