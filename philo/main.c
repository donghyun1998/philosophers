/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:53:43 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/14 21:34:27 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eat_or_die(t_philo *philo) // 이름 뭐로하지
{
	pthread_mutex_lock(philo->left);
	printf("%lld %d has taken a left fork\n",get_current_time(), philo->id);
	pthread_mutex_lock(philo->right); // 여기서 함수로 빼서 죽는 시간 확인해야 할듯
	printf("%lld %d has taken a right fork\n",get_current_time(), philo->id);
	if ((get_current_time() - philo->info->start_time) > philo->info->time_to_die)
	{ // 굶어죽음
		printf("%lld %d died\n",get_current_time(), philo->id);
		philo->info->dead_philo_flag = 1; // 뮤텍스 필없음 초기화 해주는거기때문에 자원소모 ㄴ
	}
	else // 먹을 때
	{
		printf("%lld %d is eating\n",get_current_time(), philo->id);
		usleep(philo->info->time_to_eat);
		philo->eat_cnt++;
		philo->last_eat_time = get_current_time();
	}
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
	if (philo->info->dead_philo_flag)
		return (KO);
	return (OK);
}

void *thread_func(void *philos)
{
	t_philo *philo;

	philo = (t_philo *)philos;
	while (philo->info->must_eat_count == 0 || philo->eat_cnt < philo->info->must_eat_count)
	{
		if (philo->id % 2 == 1)
			usleep(1000); // 짝수 먼저 가즈아
		//======== critical section ====================================================================
		if (eat_or_die(philo) == KO)
			return (NULL);
		//======== critical section ====================================================================
		printf("%lld %d is sleeping\n",get_current_time(), philo->id);
		usleep(philo->info->time_to_sleep);
		printf("%lld %d is thinking\n",get_current_time(), philo->id);
	}
	pthread_mutex_lock(&(philo->info->mutex_of_full_philo_cnt));
	philo->info->full_philo_cnt++;
	pthread_mutex_unlock(&(philo->info->mutex_of_full_philo_cnt));
	return (NULL);
}

void	init_thread(t_philo *philos, t_info *info)
{
	int i;

	i = -1;
	while (++i < info->num_of_philos)
	{
		pthread_create(&(philos->thread_id), NULL, thread_func, &philos[i]);
		pthread_detach(philos->thread_id); // 이거맞나...
	}
	// 여기부터 모니터링 쓰레드로 쓴다. 그냥 필로 쓰레드에서 찍어도 될거같은데 굳이 모니터링스레드가 필요한가?
	while (1)
	{
		// printf("%d\n", info->full_philo_cnt);
		if (info->dead_philo_flag)
			exit(1); /////////// 에러 처리
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

	info = NULL;
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
