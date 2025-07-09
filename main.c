/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmouis <hmouis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:38:11 by hmouis            #+#    #+#             */
/*   Updated: 2025/07/09 15:38:19 by hmouis           ###   ########.fr       */
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
		if (!init_philos(&table, av))
		{
			free_all(&table);
			return (0);
		}
		free(table.forks);
		free(table.philo);
		free(table.all_thread);
		free(table.t_th);
	}
	return (0);
}
