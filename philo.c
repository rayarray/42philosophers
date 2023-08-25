/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:56:39 by rleskine          #+#    #+#             */
/*   Updated: 2023/08/25 22:10:23 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// use m_die to prevent data race
void	*supervisor(void *arg)
{
	t_table			*t;
	int				i;
	struct timeval	now;

	t = (t_table *)arg;
	i = 0;
	checkmutex(t->brn, PHILO_START, NULL);
	while (checkmutex(t->brn, PHILO_SATED, NULL) > 0)
	{
		pthread_mutex_lock(&t->m_die);
		if ((t->brn + i)->lastmeal.tv_sec != 0
			&& (t->brn + i)->meals != t->times_to_eat
			&& get_ms_diff(&(t->brn + i)->lastmeal, &now, 1) > t->t_starve)
		{
			//printf("died\n");
			(t->brn + i)->alive = 0;
			add_log_msg(t->brn + i, PHILO_DEAD, 0);
			break ;
		}
		pthread_mutex_unlock(&t->m_die);
		if (++i >= t->seats)
			i = 0;
		usleep(1000 / t->seats);
	}
	return (0); //pthread_exit(NULL);

}

void	*philosopher(void *arg)
{
	t_brain	*b;

	b = (t_brain *)arg;
	checkmutex(b, PHILO_START, NULL);
	gettimeofday(&(b->start), NULL);
	gettimeofday(&(b->lastmeal), NULL);
	//printf("philo %d t_think is %d\n", b->name, b->t_think);
	if (b->t_think == 0 && b->name % 2 == 1)
		rsleep(b->t_eat);
	while (checkmutex(b, PHILO_DEAD, NULL))
	{
		add_log_msg(b, PHILO_THINKING, 0);
		if (b->meals > 0 && b->t_think > 1)
			rsleep(b->t_think);
		//int i = 0;
		while (!getforks(b, 0) && checkmutex(b, PHILO_DEAD, NULL))
		{
			//printf("didn't get forks %d\n", i++);
			if (checkmutex(b, PHILO_ORDER66, NULL) == 1)
				pthread_exit(NULL);
			usleep(50);
		}
		checkmutex(b, PHILO_DEAD, &b->lastmeal);
		add_log_msg(b, PHILO_EATING, b->meals);
		rsleep(b->t_eat);
		dropforks(b);
		if (b->times_to_eat && *b->times_to_eat == b->meals
			&& add_log_msg(b, PHILO_SATED, 0))
			break ;
		add_log_msg(b, PHILO_SLEEPING, 0);
		rsleep(b->t_slp);
	}
	return (0); //pthread_exit(NULL);
}

//if (pthread_detach(*(t->phl + i)))
//	printf("Thread detach failed\n");
int	commence_philosophy(t_table *t, int i)
{
	while (i < t->seats)
		if (pthread_mutex_init(t->frk + i++, NULL))
			printf("Mutex creation error\n");
	if (pthread_mutex_init(&t->m_die, NULL))
		printf("Mutex creation error\n");
	if (pthread_mutex_init(&t->m_log, NULL))
		printf("Mutex creation error\n");
	if (pthread_mutex_init(&t->m_stop, NULL))
		printf("Mutex creation error\n");
	while (--i >= 0)
		make_brain(i, t->brn, t->frk, t->seats - 1);
	pthread_mutex_lock(&t->m_stop);
	while (++i < t->seats)
	{
		if (pthread_create(t->phl + i, NULL, philosopher, t->brn + i))
			printf("Thread creation failed\n");
	}
	if (pthread_create(&t->superv, NULL, supervisor, t))
		printf("Supervisor thread creation failed\n");
	pthread_mutex_unlock(&t->m_stop);
	usleep(250);
	return (1);
}

int	init_table(t_table *t, char **ag, int ac)
{
	t->seats = read_args(ag, 1);
	t->phl = malloc(sizeof(pthread_t) * t->seats);
	t->brn = malloc(sizeof(t_brain) * t->seats);
	t->frk = malloc(sizeof(pthread_mutex_t) * t->seats);
	if (t->phl == NULL || t->brn == NULL || t->frk == NULL)
		return (0);
	memset(t->brn, 0, sizeof(t_brain) * t->seats);
	t->times_to_eat = -1;
	if (ac == 6)
		t->times_to_eat = read_args(ag, 5);
	if (ac == 6)
		t->brn->times_to_eat = &t->times_to_eat;
	t->t_starve = read_args(ag, 2);
	t->brn->t_eat = read_args(ag, 3);
	t->brn->t_slp = read_args(ag, 4);
	t->brn->log = init_log(t->seats);
	if (t->brn->log == NULL)
		return (0);
	t->brn->m_log = &t->m_log;
	t->brn->m_die = &t->m_die;
	t->brn->m_stop = &t->m_stop;
	return (1);
}

int	main(int ac, char **ag)
{
	t_table	tbl;

	if (chk_args(ac, ag) == -1 || read_args(ag, 1) == 0)
		return (0);
	if (init_table(&tbl, ag, ac) == 1)
	{
		commence_philosophy(&tbl, 0);
		while (print_log(tbl.brn->log, &tbl.m_log))
			usleep(250);
	}
	pthread_join(tbl.superv, NULL);
	stopall(&tbl, 0);
	ac = tbl.seats;
	while (--ac >= 0)
		pthread_join(*(tbl.phl + ac), NULL);
	pthread_mutex_destroy(&tbl.m_die);
	pthread_mutex_destroy(&tbl.m_log);
	ac = -1;
	while (++ac < tbl.seats)
		pthread_mutex_destroy(tbl.frk + ac);
	free(tbl.phl);
	free(tbl.brn->log);
	free(tbl.brn);
	free(tbl.frk);
	return (0);
}
