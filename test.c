/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 17:47:23 by vduchi            #+#    #+#             */
/*   Updated: 2023/07/05 19:41:40 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

t_philo	*philo_init(t_table *table)
{
	t_philo	*philo;

	philo = &table->philos[table->count];
	table->count++;
	philo->c_eat = 0;
//	philo->last_eat = 0;
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
	return (philo);
}

void	*philo_loop(t_table *table)
{
	t_philo	*philo;

	pthread_mutex_lock(table->print);
	philo = philo_init(table);
	while (!table->is_dead)
	{
		pthread_mutex_lock(philo->left_fork);
		print_func(TAKE_FORK, table, philo);
		pthread_mutex_lock(philo->right_fork);
		print_func(TAKE_FORK, table, philo);
		print_func(EAT, table, philo);
		pthread_mutex_lock(table->setup);
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
	return (NULL);
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
	*num = n;
	return (0);
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
		return (print_help("One of the arguments is not a positive int\n"));
	if (argc == 6 && atoi_input(argv[5], &table->max_eat))
		return (print_help("One of the arguments is not a positive int\n"));
	else if (argc != 6)
		table->max_eat = -1;
	return (0);
}

int	free_malloc(t_table *table, int mode)
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
		printf("Mutex init failed\n");
	else if (mode == 2)
		printf("Thread creation failed\n");
	else if (mode == 3)
		printf("Get time error\n");
	return (1);
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
		return (free_malloc(table, 0));
	if (pthread_mutex_init(table->print, NULL) != 0)
		return (free_malloc(table, 1));
	if (pthread_mutex_init(table->setup, NULL) != 0)
		return (free_malloc(table, 1));
	i = -1;
	while (++i < table->n_philos)
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
			return (free_malloc(table, 1));
	return (0);
}
/*
void	print_pointers(t_table *table, int i)
{
	pthread_mutex_lock(table->print);
	printf("Left fork: %p\tRight fork: %p\tPhilo: %p\tNumber: %i\n", \
		table->philos[i].left_fork, table->philos[i].right_fork, &table->philos[i], i);
	pthread_mutex_unlock(table->print);
}

void	print_test(long time, t_table *table)
{
	pthread_mutex_lock(table->print);
	printf("Passed: %ld\tDifference: %ld\n", time, time - table->timers.t_start);
	pthread_mutex_unlock(table->print);
}
*/
int	start_threads(t_table *table)
{
	int				i;
	struct timeval	start_time;

	i = -1;
	gettimeofday(&start_time, NULL);
	pthread_mutex_lock(table->setup);
	table->timers.t_start = (start_time.tv_sec * 1000) + (start_time.tv_usec / 1000);
	while (++i < table->n_philos)
		table->philos[i].last_eat = table->timers.t_start;
	i = -1;
	while (++i < table->n_philos)
	{
		if (pthread_create(&table->philos[i].philo, NULL, (void *)&philo_loop, (void *)table) != 0)
			return (free_malloc(table, 2));
		pthread_detach(table->philos[i].philo);
	}
	pthread_mutex_unlock(table->setup);
	return (0);
}

void	someone_died(t_table *table, int count)
{
	long	now;

	now = get_current_time();
	if (now == -1)
	{
		printf("Get time error\n");
		return ;
	}
	pthread_mutex_lock(table->print);
	if (printf("%.4ld : %d  died\n", \
		now - table->timers.t_start, table->philos[count].n) == -1)
	{
		write(2, "Printf error\n", 13);
		return ;
	}
	table->is_dead = 1;
	pthread_mutex_unlock(table->print);
//	usleep(5);
}

/*
void	print_custom(t_table *table, char *str, int value)
{
	pthread_mutex_lock(table->print);
	printf("%s%d\n", str, value);
	pthread_mutex_unlock(table->print);
}
*/

int	check_eat(t_table *table, int eat_times)
{
	int	i;
	int	check;

	i = -1;
	check = 1;
	pthread_mutex_lock(table->setup);
	while (++i < table->n_philos - 1)
	{
		if (table->philos[i].c_eat != table->philos[i + 1].c_eat)
			check = 0;
		if (!check)
			break ;
	}
	if (check == 1 && table->philos[i].c_eat == eat_times)
		check = 0;
	pthread_mutex_unlock(table->setup);
	return (check);
}

int main(int argc, char *argv[])
{
//    int 			i = -1;
	int				count;
	int				eat_times;
	t_table			table;
	long			passed_time;
	long			started_time;

	count = 0;
	eat_times = 0;
	if (check_args(&table, argv, argc) || table_init(&table)
		|| start_threads(&table))
		return (1);
	my_sleep(200);
	started_time = get_current_time();
//	if (main_loop(table))
//		return (free_malloc(&table, 3));
	while (!table.is_dead)
	{
		if (count == table.n_philos)
			count = 0;
		passed_time = get_current_time();
		if (passed_time == -1)
		{
			printf("Get time error\n");
			return (1);
		}
		pthread_mutex_lock(table.setup);
		if (table.max_eat != -1 && check_eat(&table, eat_times))
			eat_times++;
		if (passed_time - table.philos[count].last_eat > table.timers.t_die)
			someone_died(&table, count);
		pthread_mutex_unlock(table.setup);
		count++;
	}
//	while (++i < table.n_philos)
//		pthread_join(table.philos[i].philo, NULL);
//	i = -1;
//	while (++i < table.n_philos)
//		pthread_mutex_destroy(&table.forks[i]);
//	pthread_mutex_destroy(table.print);
//	pthread_mutex_destroy(table.setup);
//	free_malloc(&table, -1);
    return (0);
}
