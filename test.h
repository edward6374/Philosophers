/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 12:23:52 by vduchi            #+#    #+#             */
/*   Updated: 2023/06/29 19:05:42 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct	s_time
{
	long	t_eat;
	long	t_sleep;
	long	t_start;
}	t_time;

typedef struct	s_philos
{
	int				n;
	int				c_eat;
	long			tv_eat;
	long			tv_sleep;
	pthread_t		philo;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}	t_philo;

typedef struct	s_table
{
	int				n_philos;
	int				count_ph;
	int				someone_is_dead;
	t_time			timers;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*setup;
	pthread_mutex_t	*print_mutex;
}	t_table;

# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define TAKE_FORK "has taken a fork"

void	my_sleep(long wait);
long	get_current_time(void);
int		print_func(char *str, t_table *table, t_philo *philo);

#endif
