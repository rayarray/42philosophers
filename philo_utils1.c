/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 18:32:40 by rleskine          #+#    #+#             */
/*   Updated: 2023/08/25 17:21:22 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_brain	*make_brain(int nbr, t_brain *b, pthread_mutex_t *f, int last)
{
	(b + nbr)->name = nbr + 1;
	(b + nbr)->left = f + nbr;
	if (nbr != last)
		(b + nbr)->right = f + nbr + 1;
	else
		(b + nbr)->right = f;
	(b + nbr)->meals = b->meals;
	(b + nbr)->times_to_eat = b->times_to_eat;
	(b + nbr)->alive = 1;
	(b + nbr)->m_log = b->m_log;
	(b + nbr)->m_die = b->m_die;
	(b + nbr)->m_stop = b->m_stop;
	(b + nbr)->log = b->log;
	(b + nbr)->t_eat = b->t_eat;
	(b + nbr)->t_slp = b->t_slp;
	if (last % 2 == 0 && b->t_slp < b->t_eat * 2)
		(b + nbr)->t_think = b->t_eat * 2 - b->t_slp;
	return (b + nbr);
}

int	getfork(t_brain *b, pthread_mutex_t *fork, int *err)
{
	*err = pthread_mutex_lock(fork);
	if (*err)
		return (printf("MUTEX LOCK ERROR %d\n", *err) * 0);
	if (checkmutex(b, PHILO_ORDER66, NULL))
	{
		pthread_mutex_unlock(fork);
		return (-1);
	}
	return (1);
}

int	getforks(t_brain *b, int err)
{
	if (b->left == b->right)
		return (0);
	if (b->name % 2 == 0 && getfork(b, b->left, &err) < 1)
		return (0);
	if (getfork(b, b->right, &err) < 1)
	{
		if (b->name % 2 == 0)
			pthread_mutex_unlock(b->left);
		return (0);
	}
	if (b->name % 2 == 1 && getfork(b, b->left, &err) < 1)
	{
		pthread_mutex_unlock(b->right);
		return (0);
	}
	return (1);
}

// int	getforks(t_brain *b, int lock)
// {
// 	if (b->left == b->right)
// 		return (0);
// 	if (b->name % 2 == 1)
// 	{
// 		lock = pthread_mutex_lock(b->right);
// 		if (lock)
// 			printf("P%d: Error code %d when locking mutex\n", b->name, lock);
// 		if (checkmutex(b, PHILO_ORDER66, NULL))
// 			return (pthread_mutex_unlock(b->right) * 0);
// 	}
// 	lock = pthread_mutex_lock(b->left);
// 	if (lock)
// 		printf("P%d: Error code %d when locking mutex\n", b->name, lock);
// 	if (checkmutex(b, PHILO_ORDER66, NULL))
// 		return (pthread_mutex_unlock(b->left) * 0);
// 	if (b->name % 2 == 0)
// 	{
// 		lock = pthread_mutex_lock(b->right);
// 		if (lock)
// 			printf("P%d: Error code %d when locking mutex\n", b->name, lock);
// 	}
// 	return (1);
// }

int	dropforks(t_brain *b)
{
	if (b->name % 2 == 0)
		pthread_mutex_unlock(b->left);
	pthread_mutex_unlock(b->right);
	if (b->name % 2 == 1)
		pthread_mutex_unlock(b->left);
	return (1);
}

void	stopall(t_table *t, int i)
{
	pthread_mutex_lock(&t->m_stop);
	while (i < t->seats)
		(t->brn + i++)->stop = 1;
	pthread_mutex_unlock(&t->m_stop);
	i = 0;
	while (i < t->seats)
	{
		if ((t->brn + i)->alive == 0)
			break ;
		i++;
	}
	if (i < t->seats)
		pthread_mutex_unlock(&t->m_die);
}