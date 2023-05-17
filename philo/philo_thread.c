/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 21:29:32 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/17 18:46:22 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	pick_up_fork(t_philo *philo)
{
	pthread_mutex_lock(philo->left);
	pthread_mutex_lock(&philo->info->mutex_of_dead_philo_flag);
	if (philo->info->dead_philo_flag == 0)
		printf("%lld %d has taken a fork\n",
			get_millisec() - philo->info->start_time, philo->id + 1);
	else
		{
			pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
			return (KO); // 나가야한 리턴값 만들어서
		}
	pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
	pthread_mutex_lock(philo->right);
	pthread_mutex_lock(&philo->info->mutex_of_dead_philo_flag);
	if (philo->info->dead_philo_flag == 0)
		printf("%lld %d has taken a fork\n",
			get_millisec() - philo->info->start_time, philo->id + 1);
	else
		{
			pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
			return (KO); // 나가야한 리턴값 만들어서
		}
	pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
	return (OK);
}

void	put_down_fork(t_philo *philo)
{
	pthread_mutex_unlock(philo->right);
	pthread_mutex_unlock(philo->left);
}


int	eat_or_die(t_philo *philo)
{
	if (pick_up_fork(philo) == KO)
		return (KO);
	pthread_mutex_lock(&(philo->mutex_of_eat));
	if ((get_millisec() - philo->last_eat_time) >= philo->info->time_to_die)
	{
		pthread_mutex_unlock(&(philo->mutex_of_eat));
		pthread_mutex_lock(&(philo->info->mutex_of_dead_philo_flag));
		if (philo->info->dead_philo_flag == 0)
		{
			printf("%lld %d died\n", get_millisec() - philo->info->start_time, philo->id + 1);
			philo->info->dead_philo_flag = 1;
		}
		pthread_mutex_unlock(&(philo->info->mutex_of_dead_philo_flag));
		put_down_fork(philo);
		return (KO);
	}
	else
	{
		philo->last_eat_time = get_millisec();
		philo->eat_cnt++;
		pthread_mutex_unlock(&(philo->mutex_of_eat));
		pthread_mutex_lock(&philo->info->mutex_of_dead_philo_flag);
		if (philo->info->dead_philo_flag == 0)
			printf("%lld %d is eating\n",
				get_millisec() - philo->info->start_time, philo->id + 1);
		else
		{
			pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
			put_down_fork(philo);
			return (KO);
		}
		pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
		msleep(philo->info->time_to_eat);
		put_down_fork(philo);
		return (OK);
	}
}

void check_leak(void) {
	system("leaks philo");
}
void	check_start_flag(t_philo *philo)
{
	pthread_mutex_lock(&(philo->info->mutex_of_start_flag));
	philo->last_eat_time = philo->info->start_time;
	philo->info->start_thread_cnt += 1;
	pthread_mutex_unlock(&(philo->info->mutex_of_start_flag));
}

void	routine(t_philo *philo)
{
	while (42)
	{
		pthread_mutex_lock(&(philo->mutex_of_eat));
		if (!(philo->info->must_eat_count == 0
				|| philo->eat_cnt < philo->info->must_eat_count)) // 배부른지 검사
		{
			pthread_mutex_lock(&(philo->info->mutex_of_full_philo_cnt));
			philo->info->full_philo_cnt++;
			pthread_mutex_unlock(&(philo->info->mutex_of_full_philo_cnt));
			// break ;
		}
		pthread_mutex_unlock(&(philo->mutex_of_eat));
		if (eat_or_die(philo) == KO)
			return ;
		pthread_mutex_lock(&philo->info->mutex_of_dead_philo_flag);
		if (philo->info->dead_philo_flag == 0)
			printf("%lld %d is sleeping\n",
				get_millisec() - philo->info->start_time, philo->id + 1);
		else
		{
			pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
			return ;
		}
		pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
		msleep(philo->info->time_to_sleep);
		pthread_mutex_lock(&philo->info->mutex_of_dead_philo_flag);
		if (philo->info->dead_philo_flag == 0)
			printf("%lld %d is thinking\n",
				get_millisec() - philo->info->start_time, philo->id + 1);
		else
		{
			pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
			return ;
		}
		pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
	}
}

void	*thread_func_philo(void *philos)
{
	t_philo	*philo;

	philo = (t_philo *)philos;
	check_start_flag(philo);
	if (philo->id % 2 == 1)
		usleep(1000);
	routine(philo);
	return (NULL);
}
