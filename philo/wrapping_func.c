/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapping_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 20:05:18 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/14 21:21:37 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	msleep(long long target_time)
{
	long long	start_time;

	start_time = get_current_time();
	while (target_time > get_current_time() - start_time)
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
	//구조체 free도 나중에
	return(1); // exit 안됨 외않되
}

long long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
