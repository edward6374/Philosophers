/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 14:40:39 by vduchi            #+#    #+#             */
/*   Updated: 2023/07/06 19:57:27 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	check_args(t_table *table, char *argv[], int argc)
{
	if (argc == 1)
		return (print_help(NULL));
	if (argc < 5 || argc > 6)
		return (print_help("Number of arguments incorrect!\n"));
	if (atoi_input(argv[1], &table->n_philos)
		|| atoi_input(argv[2], &table->timers.t_die)
		|| atoi_input(argv[3], &table->timers.t_eat)
		|| atoi_input(argv[4], &table->timers.t_sleep))
		return (print_help("One of the arguments is not a positive int or is zero\n"));
	if (argc == 6 && atoi_input(argv[5], &table->max_eat))
		return (print_help("One of the arguments is not a positive int or is zero\n"));
	else if (argc != 6)
		table->max_eat = -1;
	return (0);
}

int	table_init(t_table *table)
{
	int	i;

	table->count = 0;
	table->is_dead = 0;
	table->print = malloc(sizeof(pthread_mutex_t));
	table->setup = malloc(sizeof(pthread_mutex_t));
	table->forks = malloc(sizeof(pthread_mutex_t) * table->n_philos);
	table->philos = (t_philo *)malloc(sizeof(t_philo) * table->n_philos);
	if (!table->print || !table->setup || !table->forks || !table->philos)
		return (free_malloc(table, 0, 0));
	if (pthread_mutex_init(table->print, NULL) != 0)
		return (free_malloc(table, 1, 0));
	if (pthread_mutex_init(table->setup, NULL) != 0)
		return (free_malloc(table, 1, 1));
	i = -1;
	while (++i < table->n_philos)
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
			return (free_malloc(table, 1, 2 + i));
	return (0);
}

int	start_threads(t_table *table)
{
	int				i;
	struct timeval	start_time;

	i = -1;
	gettimeofday(&start_time, NULL);
	pthread_mutex_lock(table->setup);
	table->timers.t_start = (start_time.tv_sec * 1000) + \
		(start_time.tv_usec / 1000);
	while (++i < table->n_philos)
		table->philos[i].last_eat = table->timers.t_start;
	i = -1;
	while (++i < table->n_philos)
	{
		if (pthread_create(&table->philos[i].philo, NULL, (void *)&philo_init, \
			(void *)table) != 0)
			return (free_malloc(table, 2, 0));
	}
	pthread_mutex_unlock(table->setup);
	return (0);
}

int	main_loop(t_table *table)
{
	int				count;
	int				eat_times;
	long			passed_time;

	count = 0;
	eat_times = 0;
	while (!table->is_dead)
	{
		if (count == table->n_philos)
			count = 0;
		passed_time = get_current_time();
		pthread_mutex_lock(table->setup);
		if (table->max_eat != -1 && check_eat(table, eat_times))
			eat_times++;
		if (passed_time - table->philos[count].last_eat > table->timers.t_die)
		{
			if (someone_died(table, count))
				return (free_malloc(table, 3, 0));
		}
		pthread_mutex_unlock(table->setup);
		count++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	int		i;
	t_table	table;

	i = -1;
	if (check_args(&table, argv, argc) || table_init(&table)
		|| start_threads(&table))
		return (1);
	my_sleep(1);
	main_loop(&table);
	while (++i < table.n_philos)
		pthread_join(table.philos[i].philo, NULL);
	free_mutex(&table, 0);
	free_malloc(&table, -1, 0);
	return (0);
}
