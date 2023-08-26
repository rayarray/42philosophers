/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/26 14:30:00 by rleskine          #+#    #+#             */
/*   Updated: 2023/08/26 18:53:53 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philoinit(t_brain *b)
{
	checkmutex(b, CHECK_START, NULL);
	pthread_mutex_lock(b->m_die);
	gettimeofday(&(b->start), NULL);
	gettimeofday(&(b->lastmeal), NULL);
	pthread_mutex_unlock(b->m_die);
}

t_brain	*philostart(t_brain *b)
{
	philoinit(b);
	if (!checkmutex(b, CHK_M_DIE, NULL))
		return (NULL);
	add_log_msg(b, PHILO_THINKING, 0);
	if (b->times_to_eat && *b->times_to_eat == 0
		&& add_log_msg(b, PHILO_SATED, 0))
		return (NULL);
	if (b->t_think == 0 && b->name % 2 == 1)
		rsleep(b->t_eat);
	while (!getforks(b, 0) && checkmutex(b, CHK_M_DIE, NULL))
		if (checkmutex(b, CHK_M_STOP, NULL) == 1)
			return (NULL);
	checkmutex(b, SET_EAT_TIMER, &b->lastmeal);
	add_log_msg(b, PHILO_EATING, b->meals);
	rsleep(b->t_eat);
	dropforks(b);
	if (b->times_to_eat && *b->times_to_eat <= b->meals
		&& add_log_msg(b, PHILO_SATED, 0))
		return (NULL);
	add_log_msg(b, PHILO_SLEEPING, 0);
	rsleep(b->t_slp);
	return (b);
}
