/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 17:47:23 by vduchi            #+#    #+#             */
/*   Updated: 2023/06/29 19:32:38 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	*doSomeThing(void *args)
{
	t_table		*table;
	t_philo	*philo;

	table = (t_table *)args;
//	pthread_mutex_lock(table->setup);
	philo = &table->philos[table->count_ph];
	philo->c_eat = 0;
	philo->n = table->count_ph;
	table->count_ph++;
//	pthread_mutex_unlock(table->setup);
	if (philo->n == 0 || philo->n == 2)
		my_sleep(table->timers.t_eat);
	while (!table->someone_is_dead)
	{
		pthread_mutex_lock(philo->left_fork);
		print_func(TAKE_FORK, table, philo);
		pthread_mutex_lock(philo->right_fork);
		print_func(TAKE_FORK, table, philo);
		print_func(EAT, table, philo);
//		usleep(table->timers.t_eat * 1000);
		philo->tv_eat = get_current_time();
		philo->c_eat++;
		my_sleep(table->timers.t_eat);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		print_func(SLEEP, table, philo);
//		usleep(table->timers.t_sleep * 1000);
		my_sleep(table->timers.t_eat);
		print_func(THINK, table, philo);
//		if (philo->c_eat > 10)
//		{
//			table->someone_is_dead = 1;
//			return (clean_mem(table));
//		}
	}
	return (NULL);
}

int	print_help(void)
{
	printf("The program is called with these arguments:\n");
	printf("\t1: Number of philosophers\n");
	printf("\t2: Time for each philosohper to die without eating\n");
	printf("\t3: Time that each philosopher takes to eat\n");
	printf("\t4: Time for each philosopher to sleep\n");
	printf("\t5 (optional): Number of times that all philosophers have to eat \
			for the simulation to finish\n");
	return (1);
}

int	check_args(t_table *table, char *argv[], int argc)
{
	if (argc == 1)
		return (print_help());
	if (argc < 5 || argc > 6)
	{
		printf("Number of arguments incorrect!\n");
		return (print_help());
	}
	return (0);
}

void	print_pointers(t_table *table, int i)
{
	pthread_mutex_lock(table->print_mutex);
	printf("Left fork: %p\tRight fork: %p\tPhilo: %p\tNumber: %i\n", \
		table->philos[i].left_fork, table->philos[i].right_fork, &table->philos[i], i);
	pthread_mutex_unlock(table->print_mutex);
}

void	print_test(long time, t_table *table)
{
	pthread_mutex_lock(table->print_mutex);
	printf("Passed: %ld\tDifference: %ld\n", time, time - table->timers.t_start);
	pthread_mutex_unlock(table->print_mutex);
}

int main(int argc, char *argv[])
{
    int 			i = -1;
	t_table			table;
	long			passed_time;
	struct timeval	start_time;

	table.count_ph = 0;
	table.n_philos = 4;
	table.someone_is_dead = 0;
	table.timers.t_eat = 400;
	table.timers.t_sleep = 100;
//	table.timers.t_start = 0;
//	if (check_args(&table, argv, argc))
//		return (1);
	table.forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * table.n_philos);
	table.setup = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	table.print_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	table.philos = (t_philo *)malloc(sizeof(t_philo) * table.n_philos);
	if (pthread_mutex_init(table.print_mutex, NULL) != 0)
	{
		printf("\nMutex init failed\n");
		return (1);
	}
	if (pthread_mutex_init(table.setup, NULL) != 0)
	{
		printf("\nMutex init failed\n");
		return (1);
	}
	i = -1;
	while (++i < table.n_philos)
	{
		if (pthread_mutex_init(&table.forks[i], NULL) != 0)
		{
			printf("\nMutex init failed\n");
			return (1);
		}
//		printf("Fork no.: %i\tPointer: %p\n", i, &table.forks[i]);
	}
	i = -1;
	while (++i < table.n_philos)
	{
		if (i == 0)
		{
			table.philos[i].left_fork = &table.forks[table.n_philos - 1];
//			printf("First of the table left\n");
		}
		else
		{
			table.philos[i].left_fork = &table.forks[i - 1];
//			printf("%d of the table left\n", i + 1);
		}
		table.philos[i].right_fork = &table.forks[i];
//		printf("%d of the table right\n", i + 1);
	}
	i = -1;
	while (++i < table.n_philos)
	{
		print_pointers(&table, i);
	}
	gettimeofday(&start_time, NULL);
	table.timers.t_start = (start_time.tv_sec * 1000) + (start_time.tv_usec / 1000);
//	printf("Start: %4lu\tSec: %lu\tUsec: %i\n", table.timers.t_start, start_time.tv_sec, start_time.tv_usec);
	pthread_mutex_lock(table.setup);
	i = -1;
	while (++i < table.n_philos)
	{
        if (pthread_create(&table.philos[i].philo, NULL, &doSomeThing, &table) != 0)
		{
            printf("\ncan't create thread :[%d]", i);
			return (1);
		}
		pthread_detach(table.philos[i].philo);
    }
	pthread_mutex_unlock(table.setup);
	while (!table.someone_is_dead)
	{
		i = -1;
		while (++i < table.n_philos)
		{
			passed_time = get_current_time();
			if (passed_time == -1)
			{
				printf("Get time error\n");
				return (1);
			}
			if (passed_time - table.timers.t_start > 5000)
			{
				print_func("Someone is dead", &table, &table.philos[i]);
				table.someone_is_dead = 1;
				i = 3;
				break ;
			}
//			print_test(passed_time, &table);
		}
	}
	i = -1;
	while (++i < table.n_philos)
	{
//		pthread_join(table.philos[i].philo, NULL);
		pthread_mutex_destroy(&table.forks[i]);
//		pthread_join(table.philos[i].philo, NULL);
//		pthread_exit(table.philos[i].philo);
	}
	free(table.philos);
	pthread_mutex_destroy(table.print_mutex);
	pthread_mutex_destroy(table.setup);
    return 0;
}
