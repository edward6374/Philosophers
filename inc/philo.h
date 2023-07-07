/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 14:43:06 by vduchi            #+#    #+#             */
/*   Updated: 2023/07/07 12:27:34 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_time
{
	int		t_eat;
	int		t_die;
	int		t_sleep;
	long	t_start;
}	t_time;

typedef struct s_philos
{
	int				n;
	int				c_eat;
	long			last_eat;
	pthread_t		philo;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}	t_philo;

typedef struct s_table
{
	int				count;
	int				max_eat;
	int				is_dead;
	int				n_philos;
	t_time			timers;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*setup;
	pthread_mutex_t	*print;
}	t_table;

# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define TAKE_FORK "has taken a fork"

/* ---		Utils.c		--- */
int		ft_strlen(char *str);
int		print_help(char *str);
int		atoi_input(char *str, int *num);
int		free_mutex(t_table *table, int number);
int		free_malloc(t_table *table, int mode, int number);

/* ---		Philos.c		--- */
void	*philo_init(t_table *table);
void	philo_loop(t_table *table, t_philo *philo);
int		someone_died(t_table *table, int count);
int		check_eat(t_table *table, int eat_times);
int		print_func(char *str, t_table *table, t_philo *philo);

/* ---		Timers.c		--- */
void	my_sleep(long wait);
long	get_current_time(void);

#endif
