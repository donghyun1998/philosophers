/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 21:29:32 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/17 00:15:13 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pick_up_fork(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_lock(philo->right);
		pthread_mutex_lock(&philo->info->mutex_of_dead_philo_flag);
		if (philo->info->dead_philo_flag == 0)
			printf("%lld %d has taken a fork\n",
				get_millisec() - philo->info->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
		pthread_mutex_lock(philo->left);
		pthread_mutex_lock(&philo->info->mutex_of_dead_philo_flag);
		if (philo->info->dead_philo_flag == 0)
			printf("%lld %d has taken a fork\n",
				get_millisec() - philo->info->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
	}
	else
	{
		pthread_mutex_lock(philo->left);
		pthread_mutex_lock(&philo->info->mutex_of_dead_philo_flag);
		if (philo->info->dead_philo_flag == 0)
			printf("%lld %d has taken a fork\n",
				get_millisec() - philo->info->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
		pthread_mutex_lock(philo->right);
		pthread_mutex_lock(&philo->info->mutex_of_dead_philo_flag);
		if (philo->info->dead_philo_flag == 0)
			printf("%lld %d has taken a fork\n",
				get_millisec() - philo->info->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
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

void	check_start_flag(t_philo *philo)
{
	pthread_mutex_lock(&(philo->info->mutex_of_start_flag));
	philo->last_eat_time = philo->info->start_time;
	pthread_mutex_unlock(&(philo->info->mutex_of_start_flag));
}

int	eat_or_die(t_philo *philo)
{
	pick_up_fork(philo);
	if ((get_millisec() - philo->last_eat_time) >= philo->info->time_to_die)
	{
		pthread_mutex_lock(&(philo->info->mutex_of_dead_philo_flag));
		if (philo->info->dead_philo_flag == 0)
		{
			printf("%lld %d died\n", get_millisec() - philo->info->start_time, philo->id + 1);
			philo->info->dead_philo_flag = 1;
		}
		pthread_mutex_unlock(&(philo->info->mutex_of_dead_philo_flag));
		put_down_fork(philo);
		pthread_mutex_unlock(&(philo->mutex_of_eat));
		return (KO);
	}
	else
	{
		philo->last_eat_time = get_millisec();
		philo->eat_cnt++;
		pthread_mutex_lock(&philo->info->mutex_of_dead_philo_flag);
		if (philo->info->dead_philo_flag == 0)
			printf("%lld %d is eating\n",
				get_millisec() - philo->info->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
		msleep(philo->info->time_to_eat);
		put_down_fork(philo);
		pthread_mutex_unlock(&(philo->mutex_of_eat));
		return (OK);
	}
}

void	*thread_func_philo(void *philos)
{
	t_philo	*philo;

	philo = (t_philo *)philos;
	check_start_flag(philo);
	if (philo->id % 2 == 1)
		usleep(1000);
	while (42)
	{
		pthread_mutex_lock(&(philo->mutex_of_eat));
		if (!(philo->info->must_eat_count == 0
				|| philo->eat_cnt < philo->info->must_eat_count))
			break ;
		if (eat_or_die(philo) == KO)
			return (NULL);
		pthread_mutex_lock(&philo->info->mutex_of_dead_philo_flag);
		if (philo->info->dead_philo_flag == 0)
			printf("%lld %d is sleeping\n",
			get_millisec() - philo->info->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
		msleep(philo->info->time_to_sleep);
		pthread_mutex_lock(&philo->info->mutex_of_dead_philo_flag);
		if (philo->info->dead_philo_flag == 0)
			printf("%lld %d is thinking\n",
				get_millisec() - philo->info->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->info->mutex_of_dead_philo_flag);
		// usleep(1000);
	}
	pthread_mutex_lock(&(philo->info->mutex_of_full_philo_cnt));
	philo->info->full_philo_cnt++;
	pthread_mutex_unlock(&(philo->info->mutex_of_full_philo_cnt));
	return (NULL);
}
