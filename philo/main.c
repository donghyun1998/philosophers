/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:53:43 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/17 16:57:10 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philo_is_alive(t_philo *philos, t_info *info)
{
	int	i;

	i = -1;
	while (++i < info->num_of_philos)
	{
		pthread_mutex_lock(&(philos[i].mutex_of_eat));
		if (get_millisec() - philos[i].last_eat_time >= info->time_to_die)
		{
			pthread_mutex_unlock(&(philos[i].mutex_of_eat));
			pthread_mutex_lock(&info->mutex_of_dead_philo_flag);
			if (info->dead_philo_flag == 0)
			{
				printf("%lld %d died\n", get_millisec() - info->start_time, i + 1);
				info->dead_philo_flag = 1;
			}
			pthread_mutex_unlock(&info->mutex_of_dead_philo_flag);
			return (KO);
		}
		else
			pthread_mutex_unlock(&(philos[i].mutex_of_eat));
	}
	return (OK);
}

void	monitoring_thread(t_philo *philos, t_info *info)
{
	while (42)
	{
		pthread_mutex_lock(&(info->mutex_of_dead_philo_flag));
		if (info->dead_philo_flag)
		{
			pthread_mutex_unlock(&(info->mutex_of_dead_philo_flag));
			return ;
		}
		pthread_mutex_unlock(&(info->mutex_of_dead_philo_flag));
		pthread_mutex_lock(&(info->mutex_of_full_philo_cnt));
		if (info->full_philo_cnt == info->num_of_philos)
		{
			printf("배불러\n"); ///////
			return ;
		}
		pthread_mutex_unlock(&(info->mutex_of_full_philo_cnt));
		if (check_philo_is_alive(philos, info) == KO)
			return ;
	}
}

void	init_thread(t_philo *philos, t_info *info)
{
	int			i;
	// pthread_t	thread_id;
	int			num_of_philos;

	i = -1;
	num_of_philos = info->num_of_philos;
	pthread_mutex_lock(&info->mutex_of_start_flag);
	while (++i < num_of_philos)
	{
		pthread_create(&philos[i].thread_id , NULL, thread_func_philo, &philos[i]);
		// pthread_detach(thread_id);
	}
	info->start_time = get_millisec();
	pthread_mutex_unlock(&info->mutex_of_start_flag);
	while (1)
	{
		pthread_mutex_lock(&info->mutex_of_start_flag);
		if (info->start_thread_cnt == info->num_of_philos)
		{
			pthread_mutex_unlock(&info->mutex_of_start_flag);
			return ;
		}
		pthread_mutex_unlock(&info->mutex_of_start_flag);
	}
}

void	leaks(void)
{
	system("leaks philo");
}

int	main(int argc, char **argv)
{
	t_info			*info;
	t_philo			*philos;
	pthread_mutex_t	*forks;

	atexit(leaks);
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
	monitoring_thread(philos, info);
	int i = -1;
	while (++i < info->num_of_philos)
		pthread_join(philos[i].thread_id, NULL);// 2번째 status 처리?
	free_all(info, philos, forks);
	// destroy_all_mutex(info, philos, forks);
}
