/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:56:23 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/10 13:36:56 by donghyk2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h> // gettimeofday func
# include <pthread.h> // thread func

#define KO	1
#define OK	0
// struct timeval // tv
// {
//   long tv_sec;    // 초
//      int tv_usec; // micro초
// }
typedef struct s_info
{
	int	num_of_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	must_eat_count;
	long long start_time;////// 시작시간
	// 뮤텍스도 넣어야함
}	t_info;

typedef struct s_philo
{
	pthread_t	tid;
	int			id;
	int			eat_cnt;
}	t_philo;

int	ft_atoi(const char *str);
int	print_error(char *message);
int	print_error_and_free_philos(char *message, t_philo **philos); ///// 쓸지 안쓸지 모르겠슴

#endif
