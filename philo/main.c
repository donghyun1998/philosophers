/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:53:43 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/10 22:41:30 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int init_info_and_check_valid(int argc, char **argv, t_info *info, struct timeval *tv)
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
	info->start_time = gettimeofday(tv, NULL);// 이거 맞냐...
	return (OK);
}

int	guarded_malloc(void **addr, int size)
{
	*addr = malloc(size);
	if (!addr)
		return (KO);
	return (0);
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
	}
	return (OK);
}

void print_philo(t_philo *philos, t_info info)
{
	for (int i = 0; i < info.num_of_philo; i++)
	{
		printf("%d %d\n", philos[i].id, philos[i].eat_cnt);
	}
}



int main(int argc, char **argv) // 등신도 알아볼 수 있는 직관성 갑 함수명을 짜보자 new 동현 출발
{
	t_info info;
	t_philo *philos;
	struct timeval	tv;

	if (argc != 5 && argc != 6)
		return (print_error("argument error"));
	if (init_info_and_check_valid(argc, argv, &info, &tv) == KO)
		return (print_error("argument error"));
	if (init_philo(&philos, info) == KO)
		return (print_error("malloc error"));
	print_philo(philos, info);
	// execute_philo(philos, info);
}
