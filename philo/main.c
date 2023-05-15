/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:53:43 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/15 23:19:01 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eat_or_die(t_philo *philo) // 이름 뭐로하지
{
	pthread_mutex_lock(philo->left);
	printf("%lld %d has taken a left fork\n",get_current_millisec() - philo->info->start_time, philo->id);
	pthread_mutex_lock(philo->right); // 여기서 함수로 빼서 죽는 시간 확인해야 할듯
	printf("%lld %d has taken a right fork\n",get_current_millisec() - philo->info->start_time, philo->id); //// leftright 지우기
	if ((get_current_millisec() - philo->last_eat_time) >= philo->info->time_to_die)
	{ // 굶어죽음
		pthread_mutex_lock(&(philo->info->mutex_of_dead_philo_flag));
		philo->info->dead_philo_flag = 1;
		pthread_mutex_unlock(&(philo->info->mutex_of_dead_philo_flag));
	}
	else // 먹을 때
	{
		philo->last_eat_time = get_current_millisec();
		philo->eat_cnt++;
		printf("%lld %d is eating\n",get_current_millisec() - philo->info->start_time, philo->id);
		msleep(philo->info->time_to_eat);
	}
	pthread_mutex_unlock(philo->right);
	pthread_mutex_unlock(philo->left);
	if (philo->info->dead_philo_flag)
		return (KO);
	return (OK);
}

void	*thread_func_philo(void *philos)
{
	t_philo *philo;

	philo = (t_philo *)philos;
	if (philo->id % 2 == 1)
			usleep(1000);
	while (philo->info->must_eat_count == 0 || philo->eat_cnt < philo->info->must_eat_count)
	{
		//======== critical section ====================================================================
		if (eat_or_die(philo) == KO)
			return (NULL);
		//======== critical section ====================================================================
		printf("%lld %d is sleeping\n",get_current_millisec() - philo->info->start_time, philo->id);
		msleep(philo->info->time_to_sleep);
		printf("%lld %d is thinking\n",get_current_millisec() - philo->info->start_time, philo->id);
	}
	pthread_mutex_lock(&(philo->info->mutex_of_full_philo_cnt));
	philo->info->full_philo_cnt++;
	pthread_mutex_unlock(&(philo->info->mutex_of_full_philo_cnt));
	return (NULL);
}

// void	*thread_func_monitor(void **philos)
// {
// 	t_philo **philo;
// 	t_info	*info;
// 	int		i;

// 	info = (*philo)[0].info;
// 	philo = (t_philo **)philos;
// 	while (42)
// 	{
// 		i = -1;
// 		while (++i < info->num_of_philos)
// 		{
// 			if (get_current_millisec() - (*philo)[i].last_eat_time >= info->time_to_die)
// 			{
// 				printf("%lld %d died\n",get_current_millisec() - info->start_time, i);
// 				pthread_mutex_lock(&(info->mutex_of_dead_philo_flag));
// 				info->dead_philo_flag = 1;
// 				pthread_mutex_unlock(&(info->mutex_of_dead_philo_flag));
// 				return (NULL);
// 			}
// 		}
// 	}
// 	return (NULL);
// }

void	init_thread(t_philo *philos, t_info *info)
{
	int			i;
	pthread_t	thread_id;

	i = -1;
	while (++i < info->num_of_philos)
	{
		pthread_create(&thread_id, NULL, thread_func_philo, &philos[i]);
		pthread_detach(thread_id);
	}
	while (1)
	{
		pthread_mutex_lock(&(info->mutex_of_dead_philo_flag));
		if (info->dead_philo_flag) // 죽은애 확인
		{
			printf("에러\n");
			pthread_mutex_unlock(&(info->mutex_of_dead_philo_flag));
			return ; /////////// 에러 처리
		}
		pthread_mutex_unlock(&(info->mutex_of_dead_philo_flag));
		pthread_mutex_unlock(&(info->mutex_of_full_philo_cnt));
		if (info->full_philo_cnt == info->num_of_philos)
		{
			printf("끝\n");
			return ; ////////////
		}
		pthread_mutex_unlock(&(info->mutex_of_full_philo_cnt));
		i = -1;
		while (++i < info->num_of_philos) // 필로 개수만큼 모니터링 만들어서 확인할까..?
		{
			if (get_current_millisec() - philos[i].last_eat_time >= info->time_to_die)
			{
				printf("%lld %d died\n",get_current_millisec() - info->start_time, i);
				exit(1);
			}
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
	// free_all(info, philos, forks); // 해주면 안됨
}
