/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 10:53:29 by rleskine          #+#    #+#             */
/*   Updated: 2023/08/27 00:54:40 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	read_args(char **ag, int n)
{
	int			i;
	int			j;
	long int	value;

	value = 0;
	j = 0;
	i = 0;
	while (*ag[n] == 48 && *(ag[n] + 1) != 0)
			ag[n]++;
	while (ag[n][i])
	{
		if (ag[n][i] < 48 || ag[n][i] > 57)
			return (-1);
		i++;
	}
	while (j < i)
		value = value * 10 + (ag[n][j++] - '0');
	if (i > 10 || value > 2147483647)
		return (-1);
	return ((int)value);
}

int	chk_args(int ac, char **ag)
{
	if (ac < 5 || ac > 6)
	{
		printf("Usage: philos a b c d [e]\n\nWhere a is the number of philosop");
		printf("hers, b is the time they can survive between\nmeals (counted");
		printf(") from the start of the meal), c is the time it takes to eat\n");
		printf("and d is the time it takes to sleep. Optionally [e] can be s");
		printf("pecified as the\n");
		printf("number of meals each philosopher can eat before stopping\n");
		return (-1);
	}
	if (read_args(ag, 1) == -1 || read_args(ag, 2) == -1
		|| read_args(ag, 3) == -1 || read_args(ag, 4) == -1 ||
		(ac == 6 && read_args(ag, 5) == -1))
	{
		printf("Invalid argument. Numeric values [0-9] up to MAX_INT only!\n");
		return (-1);
	}
	if (read_args(ag, 3) == 0 || read_args(ag, 4) == 0)
	{
		printf("0 is unsafe value for time_to_sleep and time_to_eat. Exiting\n");
		return (-1);
	}
	return (1);
}

int	checkmutex(t_brain *b, int status, struct timeval *time)
{
	if (status == CHK_M_DIE)
	{
		if (pthread_mutex_lock(b->m_die))
			printf("MUTEX LOCK FAIL\n");
		if (time && ++b->meals)
			gettimeofday(time, NULL);
		return (1 + (pthread_mutex_unlock(b->m_die) * 0));
	}
	else if (status == CHK_M_LOG || status == PHILO_FINISH)
	{
		pthread_mutex_lock(b->m_log);
		if (status == PHILO_FINISH)
			b->log->sated--;
		status = b->log->sated;
		return (status + (pthread_mutex_unlock(b->m_log) * 0));
	}
	else if (status == CHK_M_STOP)
	{
		pthread_mutex_lock(b->m_stop);
		status = b->stop;
		return (status + (pthread_mutex_unlock(b->m_stop) * 0));
	}
	return (0);
}

int	get_ms_diff(struct timeval *t1, struct timeval *t2, int now)
{
	if (now)
		gettimeofday(t2, NULL);
	if (now == 10)
	{
		now = (t2->tv_sec - t1->tv_sec) * 10000;
		now += (t2->tv_usec - t1->tv_usec) / 100;
		return (now);
	}
	now = (t2->tv_sec - t1->tv_sec) * 1000;
	now += (t2->tv_usec - t1->tv_usec) / 1000;
	return (now);
}

void	rsleep(int ms)
{
	struct timeval	tv[2];

	if (ms == 0)
		return ;
	gettimeofday(&tv[0], NULL);
	if (ms > 10)
		usleep((ms - 10) * 1000);
	ms = ms * 10;
	while (get_ms_diff(&tv[0], &tv[1], 10) < ms)
		usleep(200);
}
