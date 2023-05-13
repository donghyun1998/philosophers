/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:47:00 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/13 18:17:03 by donghyk2         ###   ########.fr       */
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

int	print_error(char *message)
{
	printf("%s\n", message);
	//구조체 free도 나중에
	return(1); // exit 안됨 외않되
}

long long	get_current_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (KO);
	return(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	guarded_malloc(void **addr, int size)
{
	*addr = malloc(size);
	if (!addr)
		return (KO);
	return (OK);
}

void	free_all(int num_of_philos, t_philo *philos, pthread_mutex_t *forks) //이름 생각이안난다
{
	int	i;

	i = -1;
	if (philos)
		free (philos);
	if (forks)
	{
		while (++i < num_of_philos)
			pthread_mutex_destroy(&forks[i]);
		free(forks);
	}
}
