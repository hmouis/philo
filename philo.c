/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmouis <hmouis@1337.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:11:47 by hmouis            #+#    #+#             */
/*   Updated: 2025/07/03 15:12:08 by hmouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*track_philos(void *data)
{
	t_table *table;
	size_t i;
	size_t last_meal;

	i = 0;
	table = (t_table *)data;
	last_meal = 0;
	while (1)
	{
		i = 0;
		while(i < table->number_of_philos)
		{
			pthread_mutex_lock(&table->philo[i].lock_time);
			last_meal = table->philo[i].last_meal;
			pthread_mutex_unlock(&table->philo[i].lock_time);
			if (full_philos(table))
				break;
			if (get_current_time() - last_meal > table->time_to_die)
			{
				pthread_mutex_lock(&table->dead_lock);
				table->is_dead = true;
				pthread_mutex_unlock(&table->dead_lock);
				break;
			}
			i++;
		}
		if (check_dead(table))
		{
			pthread_mutex_lock(&table->write_lock);	
			printf("%zu %zu is dead\n", get_current_time() - last_meal, table->philo[i].id);
			pthread_mutex_unlock(&table->write_lock);
			break;
		}
		usleep(200);
	}
	return (NULL);
}

int check_meals_count(t_philo *philo)
{
	pthread_mutex_lock(philo->lock);
	if (philo->table->meals && philo->count_meals == philo->table->number_of_meals)
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

void *dinner(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;
	// if (philo->id % 2 == 0)
	// 	ft_usleep(1);
	while (1)
	{
		if (check_dead(philo->table))
			break ;
		if (check_meals_count(philo))
			break;
		print_status(philo, "is thinking", 0);
		take_forks(philo);
		count_meals_eating(philo);
		print_status(philo, "is sleeping", 1);
	}
	return (NULL);
}

void init_philos(t_table *table, char **av)
{
	size_t i;

	i = 0;
	table->number_of_philos = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
	{
		table->meals = true;
		table->number_of_meals = ft_atoi(av[5]);
	}
	else
	{
		table->number_of_meals = 0; 
		table->meals = false;
	}
	table->is_dead = false;
	table->forks = malloc(sizeof(pthread_mutex_t) * table->number_of_philos);
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
	i = 0;
	table->start_time = get_current_time();
	table->philo = malloc(sizeof(t_philo) * table->number_of_philos);
	table->all_thread = malloc(sizeof(pthread_t) * table->number_of_philos);
	table->t_th = malloc(sizeof(pthread_t) * 1);
	while (i < table->number_of_philos)
	{
		table->philo[i].id = i + 1;
		table->philo[i].r_fork = &table->forks[i];
		table->philo[i].l_fork = &table->forks[(i + 1) % table->number_of_philos];
		table->philo[i].count_meals = 0;
		table->philo[i].table = table;
		table->philo[i].full = false;
		table->philo[i].lock = &table->lock;
		table->philo[i].write_lock = &table->write_lock;
		table->philo[i].var_lock = &table->var_lock;
		table->philo[i].full_philo= &table->full_philo;
		table->philo[i].last_meal = table->start_time;
		pthread_mutex_init(&table->philo[i].lock_time, NULL);
		i++;
	}
	i = 0;
	while (i < table->number_of_philos)
	{
		pthread_create(&table->all_thread[i], NULL, dinner, &table->philo[i]);
		i++;
	}
	pthread_create(&table->t_th[0], NULL, track_philos, table);
	i = 0;
	while (i < table->number_of_philos)
	{
		pthread_join(table->all_thread[i], NULL);
		i++;
	}
	pthread_join(table->t_th[0], NULL);
}

