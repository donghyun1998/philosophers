/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:53:43 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/11 12:41:34 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_current_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

static int init_info_and_check_valid(int argc, char **argv, t_info *info)
{
	info->num_of_philo = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	if (info->num_of_philo <= 0 || info->time_to_die < 0 || info->time_to_eat < 0 || info->time_to_sleep < 0)
	{
		return (KO);
	}
	if (argc == 5)
		info->must_eat_count = 0;
	else
	{
		info->must_eat_count = ft_atoi(argv[5]);
		if (info->must_eat_count <= 0)
			return (KO);
	}
	info->start_time = get_current_time();// 이거 맞냐...
	if (info->start_time == -1)
		return (KO);
	return (OK);
}

int	guarded_malloc(void **addr, int size)
{
	*addr = malloc(size);
	if (!addr)
		return (KO);
	return (OK);
}

void	*pthread_func(void *data)
{
	return ((void *)data);
}

int init_philo(t_philo **philos, t_info info)
{
	int i;

	if (guarded_malloc((void *)philos, sizeof(t_philo) * info.num_of_philo) == KO)
		return (KO);
	i = -1;
	while (++i < info.num_of_philo)
	{
		(*philos)[i].id = i;
		(*philos)[i].eat_cnt = 0;
		if (pthread_create((*philos)[i].tid, NULL, pthread_func, 1) != OK) //// 주소가 드가야하는데 이거맞나....
			return (KO);
	}
	return (OK);
}

void print_arg(t_philo *philos, t_info info)
{
	printf("%lld\n", info.start_time);

	for (int i = 0; i < info.num_of_philo; i++)
	{
		printf("%d %d\n", philos[i].id, philos[i].eat_cnt);
	}
}



int main(int argc, char **argv) // 등신도 알아볼 수 있는 직관성 갑 함수명을 짜보자 new 동현 출발
{
	t_info info;
	t_philo *philos;

	if (argc != 5 && argc != 6)
		return (print_error("argument error"));
	if (init_info_and_check_valid(argc, argv, &info) == KO)
		return (print_error("argument error"));
	if (init_philo(&philos, info) == KO)
		return (print_error("init error"));
	print_arg(philos, info);
	// execute_philo(philos, info);
	// philo free
}
