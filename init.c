/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmouis <hmouis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 01:39:45 by hmouis            #+#    #+#             */
/*   Updated: 2025/07/10 01:27:21 by hmouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_args(t_table *table, char **av)
{
	table->number_of_philos = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
	{
		table->meals = true;
		table->number_of_meals = ft_atoi(av[5]);
		if (table->number_of_meals == 0)
			return (0);
	}
	else
	{
		table->number_of_meals = 0;
		table->meals = false;
	}
	table->is_dead = false;
	return (1);
}

int	init_mutex(t_table *table)
{
	size_t	i;

	i = 0;
	table->forks = malloc(sizeof(pthread_mutex_t) * table->number_of_philos);
	if (!table->forks)
		return (0);
	while (i < table->number_of_philos)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&table->lock, NULL);
	pthread_mutex_init(&table->write_lock, NULL);
	pthread_mutex_init(&table->var_lock, NULL);
	pthread_mutex_init(&table->t_var_lock, NULL);
	pthread_mutex_init(&table->dead_lock, NULL);
	pthread_mutex_init(&table->full_philo, NULL);
	pthread_mutex_init(&table->print_mutex, NULL);
	return (1);
}

void	init_philo_struct(t_table *table)
{
	size_t	i;

	i = 0;
	while (i < table->number_of_philos)
	{
		table->philo[i].id = i + 1;
		table->philo[i].r_fork = &table->forks[i];
		if (table->number_of_philos > 1)
			table->philo[i].l_fork = &table->forks[(i + 1)
				% table->number_of_philos];
		table->philo[i].count_meals = 0;
		table->philo[i].table = table;
		table->philo[i].full = false;
		table->philo[i].lock = &table->lock;
		table->philo[i].write_lock = &table->write_lock;
		table->philo[i].var_lock = &table->var_lock;
		table->philo[i].full_philo = &table->full_philo;
		table->philo[i].last_meal = table->start_time;
		pthread_mutex_init(&table->philo[i].lock_time, NULL);
		i++;
	}
}

void	free_all(t_table *table)
{
	if (table->philo)
		free(table->philo);
	if (table->all_thread)
		free(table->all_thread);
	if (table->t_th)
		free(table->t_th);
	if (table->forks)
		free(table->forks);
}

void *philo_one(void *data)
{
	t_philo *philo;
	
	philo = (t_philo *)data;
	printf("%zu 1 has taken a fork\n", philo->last_meal - philo->table->start_time);
	ft_usleep(philo->table->time_to_die);
	philo->last_meal = get_current_time();
	printf("%zu 1 is dead\n", philo->last_meal - philo->table->start_time);
	return (NULL);
}

void one_philo(t_table *table)
{
	if (pthread_create(&table->all_thread[0], NULL, philo_one, &table->philo[0]))
		return ;
	if (pthread_join(table->all_thread[0], NULL))
		return ;
}

int	init_philos(t_table *table, char **av)
{
	size_t	i;

	i = 0;
	if (!init_args(table, av))
		return (2);
	init_mutex(table);
	i = 0;
	table->start_time = get_current_time();
	table->philo = malloc(sizeof(t_philo) * table->number_of_philos);
	if (!table->philo)
		return (0);
	table->all_thread = malloc(sizeof(pthread_t) * table->number_of_philos);
	if (!table->all_thread)
		return (0);
	table->t_th = malloc(sizeof(pthread_t) * 1);
	if (!table->t_th)
		return (0);
	init_philo_struct(table);
	i = 0;
	if (table->number_of_philos == 1)
	{
		one_philo(table);
		return (1);
	}
	if (pthread_create(&table->t_th[0], NULL, track_philos, table))
		return (0);
	while (i < table->number_of_philos)
	{
		if (pthread_create(&table->all_thread[i], NULL, dinner,
				&table->philo[i]))
			return (0);
		i++;
	}
	i = 0;
	while (i < table->number_of_philos)
	{
		if (pthread_join(table->all_thread[i], NULL))
			return (0);
		i++;
	}
	if (pthread_join(table->t_th[0], NULL))
		return (0);
	return (1);
}
