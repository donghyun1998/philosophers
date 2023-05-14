/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapping_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 20:05:18 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/14 19:08:43 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	msleep(long long target_time)
{
	long long	start_time;

	start_time = get_current_time();
	if (start_time == -1)
	{
		usleep(target_time);
		return ; // 다른 예외처리 뭐가 있을까...
	}
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

long long	get_current_time(void) // 여기다 *status를 넣어볼까
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
