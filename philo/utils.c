/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:47:00 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/13 20:40:09 by donghyk2         ###   ########.fr       */
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
	if (result > 2147483647 || result < -2147483648 || base == 0 || base > 11 || *str)
		return (-1);
	return (result);
}

void	free_all(t_info *info, t_philo *philos, pthread_mutex_t *forks) //이름 생각이안난다
{
	int	i;
	int	num_of_philos;

	i = -1;
	if (info)
	{
		num_of_philos = info->num_of_philos;
		pthread_mutex_destroy(&(info->mutex_of_full_philo_cnt));
		free(info);
	}
	if (philos)
		free (philos);
	if (forks)
	{
		while (++i < num_of_philos)
			pthread_mutex_destroy(&forks[i]);
		free(forks);
	}
}
