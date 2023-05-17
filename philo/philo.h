/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:56:23 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/17 23:04:28 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>
# include <pthread.h>

# define KO 1
# define OK 0

typedef struct s_info
{
	int				num_of_philos;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				must_eat_count;
	long long		start_time;
	pthread_mutex_t	mutex_of_full_philo_cnt;
	int				full_philo_cnt;
	pthread_mutex_t	mutex_of_dead_philo_flag;
	int				dead_philo_flag;
	pthread_mutex_t	mutex_of_start_flag;
	int				start_thread_cnt;
}	t_info;

typedef struct s_philo
{
	pthread_t		thread_id;
	int				id;
	pthread_mutex_t	mutex_of_eat;
	int				eat_cnt;
	long long		last_eat_time;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	t_info			*info;
}	t_philo;

int			ft_atoi(char *str);
int			print_error(char *message);
int			guarded_malloc(void **addr, int size);
void		free_all(t_info *info, t_philo *philos, pthread_mutex_t *forks);
long long	get_millisec(void);
int			init_info(int argc, char **argv, t_info **info);
int			init_philos(t_philo **philos, t_info *info, pthread_mutex_t *forks);
int			init_forks(pthread_mutex_t **forks, int num_of_philos);
void		msleep(long long target_time);
void		*thread_func_philo(void *philos);
void		destroy_all_mutex(t_info *info, t_philo *philos,
				pthread_mutex_t *forks);
#endif
