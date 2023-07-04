/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 14:43:06 by vduchi            #+#    #+#             */
/*   Updated: 2023/06/19 16:42:57 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>

typedef struct	s_philo
{
	int				n;
	pthread_t		thread;
	pthread_mutex_t	action;
}	t_philo;

typedef struct	s_table
{
	int				num;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				times_eat;
	pthread_mutex_t	printf;
	t_philo			*philo;
}	t_table;

int	ft_atoi(int *arr, char *argv[], int n);

#endif
