/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapping_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 20:05:18 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/16 21:37:31 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	msleep(long long target_time)
{
	long long	start_time;

	start_time = get_millisec();
	while (target_time > get_millisec() - start_time)
		usleep(1000);
}

int	guarded_malloc(void **addr, int size)
{
	*addr = malloc(size);
	if (!addr)
		return (KO);
	return (OK);
}

int	print_error(char *message)
{
	printf("%s\n", message);
	return (1);
}

long long	get_millisec(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
