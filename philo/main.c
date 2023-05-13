/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:53:43 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/13 20:59:13 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *thread_func(void *philos)
{
	t_philo *philo;

	philo = (t_philo *)philos;
	while (philo->info->must_eat_count == 0 || philo->eat_cnt < philo->info->must_eat_count)
	{
		if (philo->id % 2 == 1)
			msleep(1000); // 짝수 먼저 가즈아
		pthread_mutex_lock(philo->left);
		pthread_mutex_lock(philo->right);
		//======== critical section ====================================================================
		if ((get_current_time() - philo->info->start_time) > philo->info->time_to_die)
		{
			printf("%d번 철학자 사망\n", philo->id); // 굶어 죽었음 처리
			pthread_mutex_unlock(philo->left);
			pthread_mutex_unlock(philo->right);
			return (NULL); // 이거 맞나?
		}
		else
		{
			msleep(philo->info->time_to_eat * 1000);
			philo->eat_cnt++;
			printf("%d번 철학자 먹었다\n", philo->id);
			philo->last_eat_time = get_current_time();
		}
		//======== critical section ====================================================================
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		msleep(philo->info->time_to_sleep * 1000);
	}
	pthread_mutex_lock(&(philo->info->mutex_of_full_philo_cnt));
	philo->info->full_philo_cnt++; // 다머금 // 이거 아닌거같은데 필로 죽으면 안됨
	pthread_mutex_unlock(&(philo->info->mutex_of_full_philo_cnt));
	printf("%d번 철학자 배불러사망\n", philo->id);
	return (NULL);
}

void init_thread(t_philo *philos, t_info *info)
{
	int i;

	i = -1;
	while (++i < info->num_of_philos)
	{
		pthread_create(&(philos->thread_id), NULL, thread_func, &philos[i]);
		pthread_detach(philos->thread_id); // 이거맞나...
	}
	// 여기부터 모니터링 쓰레드로 쓴다.
	while (1)
	{
		if (info->full_philo_cnt == info->num_of_philos)
		{
			printf("끝\n");
			exit(0); ////////////
		}
	}
}

int main(int argc, char **argv) // 등신도 알아볼 수 있는 직관성 갑 함수명을 짜보자 new 동현 출발
{
	t_info			*info;
	t_philo			*philos;
	pthread_mutex_t *forks;

	philos = NULL;
	forks = NULL;
	if (argc != 5 && argc != 6)
		return (print_error("argument error"));
	if (init_info(argc, argv, &info) == KO
		|| init_forks(&forks, info->num_of_philos) == KO
		|| init_philos(&philos, info, forks) == KO)
	{
		free_all(info, philos, forks);
		return (print_error("init error"));
	}
	init_thread(philos, info);
	free_all(info, philos, forks);
}
