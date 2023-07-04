/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 15:09:06 by vduchi            #+#    #+#             */
/*   Updated: 2023/06/19 16:42:56 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	ft_atoi(int *arr, char *argv[], int n)
{
	int		i;
	int		j;
	long	num;

	i = 0;
	while (++i <= n)
	{
		j = -1;
		num = 0;
		while (argv[i][++j])
		{
			num = num * 10 + (argv[i][j] - '0');
			if (argv[i][j] > 57 || argv[i][j] < 48 || num > 2147483647)
			{
				if (num > 2147483647)
					printf("One of the arguments is bigger than an int\n");
				else
					printf("One of the arguments is not a positive number\n");
				return (1);
			}
		}
		arr[i - 1] = (int)num;
	}
	return (0);
}

