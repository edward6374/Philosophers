/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 14:40:39 by vduchi            #+#    #+#             */
/*   Updated: 2023/06/19 16:42:52 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	*run_philo(void *args)
{
	t_table	*table;

	table = (t_table *)args;
	while (1)
	{

	}
	return (NULL);
}

int	start_program(int *arr, int n)
{
	int		i;
	int		err;
	t_table	table;

	table.num = arr[0];
	table.t_die = arr[1];
	table.t_eat = arr[2];
	table.t_sleep = arr[3];
	if (n == 5)
		table.times_eat = arr[4];
	else
		table.times_eat = 0;
	table.philo = (t_philo *)malloc(sizeof(t_philo) * (arr[0] + 1));
	if (!table.philo)
	{
		printf("Malloc failure\n");
		return (1);
	}
	if (pthread_mutex_init(&(table.printf), NULL))
	{
		printf("Mutex init failure\n");
		return (1);
	}
	i = -1;
	while (++i < n)
	{
		table.philo[i].n = i + 1;
		err = pthread_create(&(table.philo[i].thread), NULL, run_philo, &table);
		if (err)
		{
			printf("Error creating thread\n");
			return (1);
		}
	}
	i = -1;
	while (++i < n)
	{
		pthread_join(table.philo[i].thread, NULL);
	}
//	(void)arr;
//	n = 0;
	return (0);
}

int	main(int argc, char *argv[])
{
	int	arr[5];

	if (argc == 1)
		printf("This program needs this arguments: \n\
			\t1:\tNumber of philosophers\n\
			\t2:\tTime for any philosopher to die\n\
			\t3:\tTime for any philosopher to eat\n\
			\t4:\tTime for any philosopher to sleep\n\
			\t5:\tNumber of times all the philosophers have eaten (optional)\n");
	else if (argc < 5)
		printf("Too few arguments\n");
	else if (argc > 6)
		printf("Too many arguments\n");
	else
		if (!ft_atoi(arr, argv, argc - 1))
			start_program(arr, argc - 1);
	return (0);
}
