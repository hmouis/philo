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
			pthread_mutex_lock(&table->t_var_lock);
			if (table->philo[i].count_meals == 0)
			{
				pthread_mutex_unlock(&table->t_var_lock);
				i++;
				continue;
			}
			pthread_mutex_unlock(&table->t_var_lock);
			pthread_mutex_lock(table->philo->var_lock);
			last_meal = table->philo[i].last_meal;
			pthread_mutex_unlock(table->philo->var_lock);
			if (get_current_time() - last_meal > table->time_to_die)
			{
				pthread_mutex_lock(&table->dead_lock);
				table->is_dead = true;
				pthread_mutex_unlock(&table->dead_lock);
				break;
			}
			i++;
		}
		if (table->is_dead == true)
		{
			pthread_mutex_lock(&table->lock);	
			printf("%zu %zu is dead\n", get_current_time() - table->philo[i].last_meal, table->philo[i].id);
			pthread_mutex_unlock(&table->lock);	
			break;
		}
	}
	return (NULL);
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

void take_forks(t_philo *philo)
{
	// if (philo->id % 2 == 0 && philo->count_meals > 0)
	// 	ft_usleep((philo->table->time_to_die - philo->table->time_to_eat - philo->table->time_to_sleep) / 2);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->l_fork);
		printf("%zu %zu has taken a fork\n", get_current_time() - philo->table->start_time, philo->id);
		pthread_mutex_lock(philo->r_fork);
		printf("%zu %zu has taken a fork\n", get_current_time() - philo->table->start_time, philo->id);
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		printf("%zu %zu has taken a fork\n", get_current_time() - philo->table->start_time, philo->id);
		pthread_mutex_lock(philo->l_fork);
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
	pthread_mutex_lock(philo->var_lock);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(philo->var_lock);
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
	else if (flag == 2)
	{
		ft_usleep(philo->table->time_to_eat);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
	}
}

void *dinner(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (1)
	{
		pthread_mutex_lock(&philo->table->dead_lock);
		if (philo->table->is_dead == true)
		{
			pthread_mutex_unlock(&philo->table->dead_lock);
			break;
		}
		pthread_mutex_unlock(&philo->table->dead_lock);
		pthread_mutex_lock(philo->lock);
		if (philo->table->meals && philo->count_meals == philo->table->number_of_meals)
		{
			pthread_mutex_lock(philo->var_lock);
			philo->full = true;
			pthread_mutex_unlock(philo->var_lock);
			pthread_mutex_unlock(philo->lock);
			break;
		}
		pthread_mutex_unlock(philo->lock);
		print_status(philo, "thinking", 0);
		take_forks(philo);
		count_meals_eating(philo);
		get_last_meals_time(philo);
		print_status(philo, "eating", 2);
		print_status(philo, "sleeping", 1);
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
	while (i++ < table->number_of_philos)
	pthread_mutex_init(&table->forks[i], NULL);
	pthread_mutex_init(&table->lock, NULL);
	pthread_mutex_init(&table->write_lock, NULL);
	pthread_mutex_init(&table->var_lock, NULL);
	pthread_mutex_init(&table->t_var_lock, NULL);
	pthread_mutex_init(&table->dead_lock, NULL);
	i = 0;
	table->start_time = get_current_time();
	table->philo = malloc(sizeof(t_philo) * table->number_of_philos);
	table->all_thread = malloc(sizeof(pthread_t) * table->number_of_philos);
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
	pthread_create(&table->t_th, NULL, track_philos, table);
	i = 0;
	while (i < table->number_of_philos)
	{
		pthread_join(table->all_thread[i], NULL);
		i++;
	}
	pthread_join(table->t_th, NULL);
}



















