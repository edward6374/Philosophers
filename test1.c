/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 12:26:20 by vduchi            #+#    #+#             */
/*   Updated: 2023/06/29 19:05:19 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

long	get_current_time(void)
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
	pthread_mutex_lock(table->print_mutex);
	if (printf("%.4ld : %d  %s\n", \
		now - table->timers.t_start, philo->n + 1, str) == -1)
	{
		write(2, "Printf error\n", 13);
		return (1);
	}
	pthread_mutex_unlock(table->print_mutex);
	return (0);
}

void	my_sleep(long wait)
{
	wait += get_current_time();
	while (wait >= get_current_time())
		usleep (100);
}
