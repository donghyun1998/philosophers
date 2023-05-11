/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyk2 <donghyk2@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:56:23 by donghyk2          #+#    #+#             */
/*   Updated: 2023/05/11 12:47:12 by donghyk2         ###   ########.fr       */
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

typedef struct s_info
{
	int			num_of_philo;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			must_eat_count;
	// 여기다 fork 배열 만들면 필로끼리 접근 가능한디....
	long long	start_time;
	// 뮤텍스도 넣어야함
}	t_info;

typedef struct s_philo
{
	pthread_t	tid;
	int			id;
	int			eat_cnt;
	int			left_fork;
	int			right_fork; // 이거도 주소로 두면 그냥 접근 가능해서 안됨 인자로 줘야함
}	t_philo;

int	ft_atoi(char *str);
int	print_error(char *message);

#endif
