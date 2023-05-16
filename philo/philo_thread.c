/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 21:29:32 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/16 21:30:27 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pick_up_fork(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_lock(philo->right); // 여기서 함수로 빼서 죽는 시간 확인해야 할듯
		printf("%lld %d has taken a fork\n",get_current_millisec() - philo->info->start_time, philo->id + 1);
		pthread_mutex_lock(philo->left);
		printf("%lld %d has taken a fork\n",get_current_millisec() - philo->info->start_time, philo->id + 1);
	}
	else
	{
		pthread_mutex_lock(philo->left);
		printf("%lld %d has taken a fork\n",get_current_millisec() - philo->info->start_time, philo->id + 1);
		pthread_mutex_lock(philo->right); // 여기서 함수로 빼서 죽는 시간 확인해야 할듯
		printf("%lld %d has taken a fork\n",get_current_millisec() - philo->info->start_time, philo->id + 1); //// leftright 지우기
	}
}

void	put_down_fork(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
	}
	else
	{
		pthread_mutex_unlock(philo->right);
		pthread_mutex_unlock(philo->left);
	}
}


int	eat_or_die(t_philo *philo) // 이름 뭐로하지
{
	int	dead_flag;

	dead_flag = 0;
	pick_up_fork(philo);
	if ((get_current_millisec() - philo->last_eat_time) >= philo->info->time_to_die)
	{ // 굶어죽음
		pthread_mutex_lock(&(philo->info->mutex_of_dead_philo_flag));
		philo->info->dead_philo_flag = 1;
		pthread_mutex_unlock(&(philo->info->mutex_of_dead_philo_flag));
		dead_flag = 1;
	}
	else // 먹을 때
	{
		philo->last_eat_time = get_current_millisec();
		philo->eat_cnt++;
		printf("%lld %d is eating\n",get_current_millisec() - philo->info->start_time, philo->id + 1);
		msleep(philo->info->time_to_eat);
	}
	pthread_mutex_unlock(&(philo->mutex_of_eat));
	put_down_fork(philo);
	if (dead_flag)
		return (KO);
	return (OK);
}

void	check_start_flag(t_philo *philo)
{
	pthread_mutex_lock(&(philo->info->mutex_of_start_flag));
	philo->last_eat_time = philo->info->start_time;
	pthread_mutex_unlock(&(philo->info->mutex_of_start_flag));
}

void	*thread_func_philo(void *philos)
{
	t_philo *philo;

	philo = (t_philo *)philos;

	check_start_flag(philo);
	if (philo->id % 2 == 1)
		usleep(1000);
	while (42)
	{
		pthread_mutex_lock(&(philo->mutex_of_eat));
		if (!(philo->info->must_eat_count == 0 || philo->eat_cnt < philo->info->must_eat_count))
			break ;
		//======== critical section ====================================================================
		if (eat_or_die(philo) == KO)
			return (NULL);
		//======== critical section ====================================================================
		printf("%lld %d is sleeping\n",get_current_millisec() - philo->info->start_time, philo->id + 1);
		msleep(philo->info->time_to_sleep);
		printf("%lld %d is thinking\n",get_current_millisec() - philo->info->start_time, philo->id + 1);
		usleep(1000); // 데드락방지
	}
	pthread_mutex_lock(&(philo->info->mutex_of_full_philo_cnt));
	philo->info->full_philo_cnt++;
	pthread_mutex_unlock(&(philo->info->mutex_of_full_philo_cnt));
	return (NULL);
}
