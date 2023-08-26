/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 19:16:38 by rleskine          #+#    #+#             */
/*   Updated: 2023/08/26 16:55:13 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_log	*init_log(int philo)
{
	t_log	*log;

	log = (t_log *)malloc(sizeof(t_log));
	if (log == NULL)
		return (NULL);
	log->lines = 0;
	log->l = 0;
	log->lines2 = 0;
	log->l2 = LOGSIZE;
	log->i = 0;
	log->full = 0;
	log->sated = philo;
	return (log);
}

int	add_log_msg(t_brain *b, int type, int meal)
{
	pthread_mutex_lock(b->m_log);
	if (b->log->lines - b->log->l >= LOGSIZE)
		b->log->full = 1;
	((b->log->log) + b->log->l + b->log->lines)->timestamp = get_ms_diff
		(&b->start, &b->now, 1);
	((b->log->log) + b->log->l + b->log->lines)->philo = b->name;
	((b->log->log) + b->log->l + b->log->lines)->type = type;
	((b->log->log) + b->log->l + b->log->lines)->meal = meal;
	(b->log->lines)++;
	pthread_mutex_unlock(b->m_log);
	return (1);
}

int	ya_power(int nbr, int pow)
{
	int	i;

	i = nbr;
	if (pow == 0)
		return (1);
	while (pow-- > 1)
		i = i * nbr;
	return (i);
}

int	nbr2str(int len, int nbr, char *s)
{
	int	i;
	int	dgts;

	i = nbr;
	dgts = 0;
	while (i > 0)
	{
		dgts++;
		i = i / 10;
	}
	if (dgts == 0)
	dgts = 1;
	i = len;
	while (i-- > dgts)
		*(s++) = ' ';
	if (dgts > len)
		len = dgts;
	while (dgts-- > 0)
	{
		i = (nbr / ya_power(10, dgts));
		*(s++) = '0' + i;
		nbr -= i * ya_power(10, dgts);
	}
	return (len);
}
