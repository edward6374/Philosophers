/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 16:11:35 by vduchi            #+#    #+#             */
/*   Updated: 2023/07/06 19:52:02 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	print_pointers(t_table *table, int i)
{
	pthread_mutex_lock(table->print);
	printf("Left fork: %p\tRight fork: %p\tPhilo: %p\tNumber: %i\n", \
		table->philos[i].left_fork, table->philos[i].right_fork, &table->philos[i], i);
	pthread_mutex_unlock(table->print);
}

void	print_custom(t_table *table, char *str)
{
	pthread_mutex_lock(table->print);
	printf("%s", str);
	pthread_mutex_unlock(table->print);
}

int	someone_died(t_table *table, int count)
{
	long	now;

	now = get_current_time();
	pthread_mutex_lock(table->print);
	if (printf("%.4ld : %d  died\n", \
		now - table->timers.t_start, table->philos[count].n) == -1)
	{
		write(2, "Printf error\n", 13);
		return (1);
	}
	table->is_dead = 1;
	pthread_mutex_unlock(table->print);
	return (0);
}

int	check_eat(t_table *table, int eat_times)
{
	int	i;
	int	check;

	i = -1;
	check = 1;
	if (table->n_philos == 1)
		return (0);
	while (++i < table->n_philos - 1)
	{
		if (table->philos[i].c_eat != table->philos[i + 1].c_eat)
			check = 0;
		if (!check)
			break ;
	}
	if (check == 1 && table->philos[i].c_eat == eat_times)
		check = 0;
	return (check);
}

int	print_func(char *str, t_table *table, t_philo *philo)
{
	long	now;

	pthread_mutex_lock(table->print);
	now = get_current_time();
	if (table->is_dead)
	{
		pthread_mutex_unlock(table->print);
		return (0);
	}
	if (printf("%.4ld : %d  %s\n", now - table->timers.t_start, \
		philo->n, str) == -1)
	{
		write(2, "Printf error\n", 13);
		return (1);
	}
	pthread_mutex_unlock(table->print);
	return (0);
}

void	*philo_init(t_table *table)
{
	t_philo	*philo;

	pthread_mutex_lock(table->print);
	philo = &table->philos[table->count];
	table->count++;
	philo->c_eat = 0;
	philo->last_eat = table->timers.t_start;
	philo->n = table->count;
	if (philo->n == 1)
		philo->left_fork = &table->forks[table->n_philos - 1];
	else
		philo->left_fork = &table->forks[philo->n - 2];
	philo->right_fork = &table->forks[philo->n - 1];
	pthread_mutex_unlock(table->print);
	pthread_mutex_lock(table->setup);
	pthread_mutex_unlock(table->setup);
	if (philo->n % 2 == 0)
		my_sleep(table->timers.t_eat);
	philo_loop(table, philo);
	return (NULL);
}

void	philo_loop(t_table *table, t_philo *philo)
{
	pthread_mutex_lock(table->setup);
	while (!table->is_dead)
	{
		pthread_mutex_unlock(table->setup);
		pthread_mutex_lock(philo->left_fork);
		print_func(TAKE_FORK, table, philo);
		if (philo->right_fork == philo->left_fork)
			break ;
		pthread_mutex_lock(philo->right_fork);
		print_func(TAKE_FORK, table, philo);
		pthread_mutex_lock(table->setup);
		print_func(EAT, table, philo);
		philo->c_eat++;
		philo->last_eat = get_current_time();
		pthread_mutex_unlock(table->setup);
		my_sleep(table->timers.t_eat);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		print_func(SLEEP, table, philo);
		my_sleep(table->timers.t_sleep);
		print_func(THINK, table, philo);
		if (philo->c_eat == table->max_eat)
			break ;
	}
	pthread_mutex_unlock(table->setup);
}
