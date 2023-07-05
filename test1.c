/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 12:26:20 by vduchi            #+#    #+#             */
/*   Updated: 2023/07/05 19:39:43 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

long	get_current_time()
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) != 0)
		return (-1);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	print_func(char *str, t_table *table, t_philo *philo)
{
	long	now;

	now = get_current_time();
	if (now == -1)
	{
		printf("Get time error\n");
		return (1);
	}
	pthread_mutex_lock(table->print);
//	pthread_mutex_lock(table->setup);
	if (table->is_dead)
	{
		pthread_mutex_unlock(table->print);
//		pthread_mutex_unlock(table->setup);
		return (0);
	}
//	pthread_mutex_unlock(table->setup);
	if (printf("%.4ld : %d  %s\n", now - table->timers.t_start, philo->n, str) == -1)
	{
		write(2, "Printf error\n", 13);
		return (1);
	}
	usleep(5);
	pthread_mutex_unlock(table->print);
	return (0);
}

void	my_sleep(long wait)
{
	wait += get_current_time();
	while (wait >= get_current_time())
		usleep (100);
}
