/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmouis <hmouis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:38:11 by hmouis            #+#    #+#             */
/*   Updated: 2025/07/10 01:03:23 by hmouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_table	table;

	if (!pars_args(ac, av))
		printf("Error: Invalide args\n");
	else
	{
		if (init_philos(&table, av) == 2)
		{
			free_all(&table);
			return (0);
		}
		free_all(&table);
		pthread_mutex_destroy(&table.write_lock);	
		pthread_mutex_destroy(&table.lock);	
		pthread_mutex_destroy(&table.var_lock);	
		pthread_mutex_destroy(&table.t_var_lock);	
		pthread_mutex_destroy(&table.dead_lock);
		pthread_mutex_destroy(&table.full_philo);
		pthread_mutex_destroy(&table.print_mutex);
	}
	return (0);
}
