/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 16:11:06 by vduchi            #+#    #+#             */
/*   Updated: 2023/07/06 19:42:47 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	free_mutex(t_table *table, int number)
{
	int	i;
	int	limit;

	i = -1;
	if (!number)
	{
		pthread_mutex_destroy(table->print);
		pthread_mutex_destroy(table->setup);
		while (++i < table->n_philos)
			pthread_mutex_destroy(&table->forks[i]);
	}
	else
	{
		pthread_mutex_destroy(table->print);
		if (number >= 2)
		{
			pthread_mutex_destroy(table->setup);
			limit = number - 2;
			while (++i < limit)
				pthread_mutex_destroy(&table->forks[i]);
		}
	}
	return (1);
}

int	free_malloc(t_table *table, int mode, int number)
{
	if (table->forks)
		free(table->forks);
	if (table->print)
		free(table->print);
	if (table->setup)
		free(table->setup);
	if (table->philos)
		free(table->philos);
	if (!mode)
		printf("Malloc error\n");
	else if (mode == 1)
	{
		free_mutex(table, number + 1);
		printf("Mutex init failed\n");
	}
	else
	{
		if (mode == 2)
			printf("Thread creation failed\n");
		else if (mode == 3)
			printf("Printf error\n");
		free_mutex(table, table->n_philos + 2);
	}
	return (1);
}

int	print_help(char *str)
{
	if (str)
		printf("\033[1;31m%s\033[0m\n", str);
	printf("\033[0;32mThe program is called with these arguments");
	printf(" (all off them must be positive integers):\n");
	printf("\t1: Number of philosophers\n");
	printf("\t2: Time for each philosohper to die without eating\n");
	printf("\t3: Time that each philosopher takes to eat\n");
	printf("\t4: Time for each philosopher to sleep\n");
	printf("\t5 (optional): Number of times that all philosophers have to eat");
	printf(" for the simulation to finish\033[0m\n");
	return (1);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	atoi_input(char *str, int *num)
{
	int		i;
	long	n;

	i = -1;
	n = 0;
	while (str[++i])
		if (str[i] < 48 || str[i] > 57)
			return (1);
	i = -1;
	while (str[++i])
		n = (n * 10) + (str[i] - '0');
	if (n > 2147483647 || n < -2147483648)
		return (1);
	if (n == 0)
		return (1);
	*num = n;
	return (0);
}
