/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:47:00 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/17 15:03:43 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(char *str)
{
	int			minus;
	long long	result;
	int			base;

	base = 0;
	result = 0;
	minus = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '-' || *str == '+')
		if (*str++ == '-')
			minus = -1;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + *str++ - '0';
		base++;
	}
	result *= minus;
	if (result > 2147483647 || result < -2147483648
		|| base == 0 || base > 11 || *str)
		return (-1);
	return (result);
}

void	destroy_all_mutex(t_info *info, t_philo *philos, pthread_mutex_t *forks)
{
	int	i;
	int	num_of_philos;

	if (info)
	{
		num_of_philos = info->num_of_philos;
		pthread_mutex_destroy(&(info->mutex_of_full_philo_cnt));
		pthread_mutex_destroy(&(info->mutex_of_dead_philo_flag));
		pthread_mutex_destroy(&(info->mutex_of_start_flag));
	}
	i = -1;
	if (philos)
		while (++i < num_of_philos)
			pthread_mutex_destroy(&(philos[i].mutex_of_eat));
	i = -1;
	if (forks)
		while (++i < num_of_philos)
			pthread_mutex_destroy(&forks[i]);
}


void	free_all(t_info *info, t_philo *philos, pthread_mutex_t *forks)
{
	destroy_all_mutex(info, philos, forks);
	if (info)
		free(info);
	if (philos)
		free (philos);
	if (forks)
		free(forks);
}
