/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:53:43 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/12 17:49:40 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void print_arg(t_philo *philos, t_info info)
{
	printf("%lld\n", info.start_time);

	for (int i = 0; i < info.num_of_philos; i++)
	{
		printf("%d %d\n", philos[i].id, philos[i].eat_cnt);
	}
}

int main(int argc, char **argv) // 등신도 알아볼 수 있는 직관성 갑 함수명을 짜보자 new 동현 출발
{
	t_info info;
	t_philo *philos;
	pthread_mutex_t	*forks;

	philos = NULL;
	forks = NULL;
	if (argc != 5 && argc != 6)
		return (print_error("argument error"));
	if (init_info(argc, argv, &info) == KO
		|| init_forks(&forks, info.num_of_philos) == KO
		|| init_philos(&philos, info.num_of_philos, forks) == KO)
	{
		free_all(philos, forks);
		return (print_error("init error"));
	}
	print_arg(philos, info);
	// execute_philo(philos, info);
	free_all(philos, forks);
}
