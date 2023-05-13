/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:53:43 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/13 18:28:28 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*thread_func(void *philos)
{
	t_philo	*philo;

	philo = (t_philo *)philos;
	if (philo->id % 2)
		usleep(1000); // 짝수 먼저 가즈아
	pthread_mutex_lock(philo->left);
	pthread_mutex_lock(philo->right);
	//======== critical section =============
	if ((get_current_time() - philo->info->start_time) > philo->info->time_to_die)
	{
		;// 굶어 죽었음 처리

	}
	else
	{
		usleep(philo->info->time_to_eat * 1000);
		philo->eat_cnt++;
		philo->last_eat_time = get_current_time();
	}
	//======== critical section =============
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
}

void	init_thread(t_philo *philos, t_info *info)
{
	int	i;

	i = -1;
	while (++i < info->num_of_philos)
		pthread_create(&(philos->thread_id), NULL, thread_func, &philos[i]);
}

int main(int argc, char **argv) // 등신도 알아볼 수 있는 직관성 갑 함수명을 짜보자 new 동현 출발
{
	t_info			info;
	t_philo			*philos;
	pthread_mutex_t	*forks;

	philos = NULL;
	forks = NULL;
	if (argc != 5 && argc != 6)
		return (print_error("argument error"));
	if (init_info(argc, argv, &info) == KO
		|| init_forks(&forks, info.num_of_philos) == KO
		|| init_philos(&philos, &info, forks) == KO)
	{
		free_all(info.full_philo_cnt, philos, forks);
		return (print_error("init error"));
	}
	init_thread(philos, &info);
	//monitoring thread 어케하지...
	free_all(info.full_philo_cnt, philos, forks);
}
