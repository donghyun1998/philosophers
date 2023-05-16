/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:31:47 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/16 20:33:56 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int init_info(int argc, char **argv, t_info **info)
{
	if (guarded_malloc((void **)info, sizeof(t_info)) == KO)
		return (KO);
	(*info)->num_of_philos = ft_atoi(argv[1]);
	(*info)->time_to_die = ft_atoi(argv[2]);
	(*info)->time_to_eat = ft_atoi(argv[3]);
	(*info)->time_to_sleep = ft_atoi(argv[4]);
	if ((*info)->num_of_philos <= 0 || (*info)->time_to_die < 0
		|| (*info)->time_to_eat < 0 || (*info)->time_to_sleep < 0)
		return (KO);
	if (argc == 5)
		(*info)->must_eat_count = 0;
	else
	{
		(*info)->must_eat_count = ft_atoi(argv[5]);
		if ((*info)->must_eat_count <= 0)
			return (KO);
	}
	// (*info)->start_time = get_current_millisec();
	(*info)->full_philo_cnt = 0;
	(*info)->dead_philo_flag = 0;
	if (pthread_mutex_init(&((*info)->mutex_of_full_philo_cnt), NULL) != OK)
		return (KO);
	if (pthread_mutex_init(&((*info)->mutex_of_dead_philo_flag), NULL) != OK)
		return (KO);
	if (pthread_mutex_init(&((*info)->mutex_of_start_flag), NULL) != OK)
		return (KO);
	return (OK);
}

int init_philos(t_philo **philos, t_info *info, pthread_mutex_t *forks)
{
	int i;

	if (guarded_malloc((void **)philos, sizeof(t_philo) * info->num_of_philos) == KO)
		return (KO);
	i = -1;
	while (++i < info->num_of_philos)
	{
		(*philos)[i].id = i;
		// (*philos)[i].last_eat_time = info->start_time;
		(*philos)[i].eat_cnt = 0;
		if (pthread_mutex_init(&((*philos)[i].mutex_of_eat), NULL) != OK)
			return (KO);
		(*philos)[i].left = &forks[i % info->num_of_philos];
		(*philos)[i].right = &forks[(i + 1) % info->num_of_philos];
		(*philos)[i].info = info;
		(*philos)[i].num_of_philos = info->num_of_philos;
		(*philos)[i].time_to_die = info->time_to_die;
		(*philos)[i].time_to_eat = info->time_to_eat;
		(*philos)[i].time_to_sleep = info->time_to_sleep;
		(*philos)[i].must_eat_count = info->must_eat_count;
		// (*philos)[i].start_time = info->start_time;
	}
	return (OK);
}

int	init_forks(pthread_mutex_t **forks, int num_of_philos)
{
	int	i;

	if (guarded_malloc((void **)forks, sizeof(pthread_mutex_t) * num_of_philos) == KO)
		return (KO);
	i = -1;
	while (++i < num_of_philos)
		if (pthread_mutex_init(&(*forks)[i], NULL) != OK)
			return (KO);
	return (OK);
}
