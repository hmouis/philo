/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmouis <hmouis@1337.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:00:18 by hmouis            #+#    #+#             */
/*   Updated: 2025/07/03 15:17:45 by hmouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_atoi_pars(char *str)
{
	int i;
	size_t result;

	i = 0;
	result = 0;
	while (str[i] && (str[i] == 30 || (str[i] >= 9 && str[i] <= 13)))
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
		if (result > INT_MAX)
			return (0);
	}
	if (str[i] != '\0' || !i)
		return (0);
	return (1);
}

int pars_args(int ac, char **av)
{
	int i;

	i = 1;
	if (ac != 5 && ac != 6)
		return (0);
	while (av[i])
	{
		if (!ft_atoi_pars(av[i]))
			return (0);
		i++;
	}
	return (1);
}

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

