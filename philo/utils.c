/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:47:00 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/10 23:50:51 by donghyk2         ###   ########.fr       */
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

int	print_error_and_free_philos(char *message, t_philo **philos)
{
	printf("%s\n", message);
	free(philos);
	return(1); // exit 안됨 외않되
}
